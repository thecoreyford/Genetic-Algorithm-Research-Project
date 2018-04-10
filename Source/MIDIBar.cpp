/*
 ==============================================================================
 
 MIDIBar.cpp
 Created: 1 Jan 2018 10:11:19pm
 Author:  Corey Ford
 
 ==============================================================================
 */

#include "MIDIBar.h"
#include "DataLogging.h"

#define C_MAJOR_TRIAD_MASK 2193 //(2^0 + 2^4 + 2^7 + 2^11)
#define C_MAJOR_SCALE_MASK 6837 //..calculated similalry.

//==============================================================================

MIDIBar::MIDIBar()
{
    thisBar.clear();//ensure array is cleared before adding values
    
    //store note value enumeration into temporary array
    int pBeatLengths[6] = {SEMIBREVE, MINIM, CROTCHET, DOTTED_CROTCHET, QUAVER, DEMI_SEMI_QUAVER};
    
    int accumulator = 0; //start bar with zero things
    do
    {
        //create a random "beat"
        Beat randBeat;
        randBeat.beatValue = arc4random_uniform(4094) + 1; //where value is a 12 bit number
        randBeat.beatLength = pBeatLengths[arc4random_uniform(6)]; //.. and length is in array
        randBeat.increment = 0; //always start with an increment of zero: leave this up to the mutations
        
        //and new random value to the accumulator
        accumulator += randBeat.beatLength;
        //Logger::outputDebugString("----- accumulator = " + (String)accumulator + "%d -------\n"); //for debugging
        
        //if not overspilling the bar (4/4)...
        if(accumulator <= SEMIBREVE)
        {
            //add this beat to the array
            thisBar.add(randBeat);
            //Logger::outputDebugString("value = " + (String)randBeat.beatLength + "\n"); //for debugging
        }
        else //reject this value as it is too large and retry
        {
            //Logger::outputDebugString("retry..... \n"); //for debugging
            accumulator -= randBeat.beatLength;
        }
    }while(accumulator != SEMIBREVE); //SEMIBREVE filling the length of a bar
}

MIDIBar::~MIDIBar(){} //destructor dosen't do anything

//==============================================================================
//==============================================================================
/* The functions below are used for
 manipulating the GA - either as a  mutation
 or reproduction*/
//==============================================================================
//==============================================================================
void MIDIBar::mutate(int selection)
{
    Beat mutatedBeat;
    
    switch(selection)
    {
        case cMajMask:
        {
            //for the length of this bar...
            for(int i =0 ; i< thisBar.size(); i++)
            {
                mutatedBeat.beatLength = thisBar[i].beatLength; //..make our new beat the same length as that input
                
                if((thisBar[i].beatValue & C_MAJOR_TRIAD_MASK) != 0) //..then if masking this beat doesn't give a zero value
                    mutatedBeat.beatValue = (thisBar[i].beatValue & C_MAJOR_TRIAD_MASK); //set it so that it does
                
                mutatedBeat.increment = thisBar[i].increment; //set our new beat to the same increment as that input
                
                thisBar.set(i, mutatedBeat); //assign that beat to the bar

            }
            break;
        }
        case toneShift:
        {
            //for the length of this bar...
            for(int i = 0; i < thisBar.size(); i++)
            {
                mutatedBeat.beatLength = thisBar[i].beatLength; //..make our new beat the same length as that input
                
                if((thisBar[i].beatValue << 2) != 0) //if the shift up 2 bits dosen't equate to zero
                    mutatedBeat.beatValue = thisBar[i].beatValue << 2; //do so!
                
                mutatedBeat.increment = thisBar[i].increment; //set our new beat to the same increment as that input
                thisBar.set(i, mutatedBeat);
            }
            break;
        }
        case segmentRepeat: //higher chance of this occouring
        {
            //initialise storage for the motif
            Beat motif[4];
            for(int i = 0; i < 4; i++)
            {
                motif[i].beatLength = QUAVER;
                motif[i].beatValue = C_MAJOR_TRIAD_MASK;
                motif[i].increment = 0;
            }
            
            //take the 1st four bars from the current note
            for(int i = 0; i < 4; i++)
            {
                if(thisBar.size() >= 4)
                    motif[i] = thisBar[i];
            }
            
            //store the bar size
            const int currentBarSize = thisBar.size();
            
            //clear the current bar
            thisBar.clear();
            
            //replace with the motif for roughly the same size as the previous bar
            for(int length = 0; length < (currentBarSize / 4.0); length++)
            {
                for(int i =0; i < 4; i++)
                    thisBar.add(motif[i]);
            }
            
            break;
        }
        case increment:
        {
            for(int i = 0; i < thisBar.size(); i++)
            {
                int random = arc4random_uniform(3);
                mutatedBeat.beatLength = thisBar[i].beatLength; //..make our new beat the same length as that input
                mutatedBeat.beatValue = thisBar[i].beatValue; //set our new beat to the same value as that input
                switch(random)
                {
                    case 0:
                        mutatedBeat.increment = 12; //8ve
                        break;
                    case 1:
                        mutatedBeat.increment = 7; //5th
                        break;
                    case 2:
                        mutatedBeat.increment = 4; //3rd
                        break;
                }
                thisBar.set(i, mutatedBeat);
            }
        }
        default:
            break;
    }
}

//==============================================================================

void MIDIBar::crossover(Array<Beat> parentA, Array<Beat> parentB)
{
    //set points of incision for each parent
    int incisionA, incisionB;
    incisionA = parentA.size() / 2;
    incisionB = parentB.size() / 2;
    
    //remove chunks from each parent
    parentA.removeRange(incisionA, parentA.size());
    parentB.removeRange(0, incisionB);
    
    //place parent B onto the end of parent A
    parentA.addArray(parentB, parentB.size());
    
    thisBar.clear(); //ensure this bar is cleared
    thisBar = parentA; //replace with parent A
}

//==============================================================================

int MIDIBar::selfAssess(Array<Beat> previousBar)
{
    int currentFitness = 0;
    
    //__________1st beat includes notes within c major scale__________//
    //for every bit of our beats value
    for(int x = 1; x <= 13; ++x)
    {
        int mask = 1 << (x-1); //1 to the power of current bit being checked
        //if both the bar value and the cmajor value are on for this bit increment current fitness
        if(((thisBar[0].beatValue & mask) & (C_MAJOR_SCALE_MASK & mask)) == mask)
        {
            currentFitness+=3;
        }
    }
    
    //__________3rd beat includes notes in c major scale?__________//
    //for every bit of our beats value
    for(int x = 1; x <= 13; ++x)
    {
        int mask = 1 << (x-1); //1 to the power of current bit being checked
        //if both the bar value and the cmajor value are on for this bit increment current fitness
        if(((thisBar[0].beatValue & mask) & (C_MAJOR_SCALE_MASK & mask)) == mask)
        {
            currentFitness+=2; //increase by less than previous value
        }
    }
    
    //__________1st massive pitch jumps__________//
    /*to keep things simple we are going to peanalise anything over the octave down to 
        up a fith in range decrementally*/
    for(int i = 0; i < thisBar.size(); i++)
    {
        if(thisBar[i].increment > 7)
        {
            currentFitness--;
            if(thisBar[i].increment >= 12)
            {
                currentFitness--;
                if(thisBar[i].increment >= 13)
                {
                    currentFitness--;
                }
            }
        }
    }
    
    //__________1st patten matching (repeats)__________//
    /*if 1st four beats equal the next four beats - mirroring our mutations*/
    for(int i = 0; i < 4; i++)
    {
        if(thisBar.size() >= 4)
            if(thisBar[i].beatValue == thisBar[i+4].beatValue)
                currentFitness++;
            
    }
    
    //__________1st suspensions__________//
    for(int i = 1; i < thisBar.size(); i++)
    {
        //for every bit of our beats value
        for(int x = 1; x <= 13; ++x)
        {
            int mask = 1 << (x-1); //1 to the power of current bit being checked
            
            //if this bar is in the cmajor scale...
            if(((thisBar[i].beatValue & mask) & (C_MAJOR_SCALE_MASK & mask)) == mask)
            {
                //give a point cause of generosity...
                currentFitness++;
                //see if the previous chord is a suspension (upwards suspesion to be particular)
                int suspensionMask = C_MAJOR_SCALE_MASK << 1;
                
                if(((previousBar[i].beatValue & mask) & (suspensionMask & mask)) == mask)
                    currentFitness++; //even more points
            }
        }

    }
    
    //__________1st long notes outside of scale__________//
    for(int i = 1; i < thisBar.size(); i++)
    {
        //for every bit of our beats value
        for(int x = 1; x <= 13; ++x)
        {
            int mask = 1 << (x-1); //1 to the power of current bit being checked
            
            //if this bar is in the cmajor scale...
            if(((thisBar[i].beatValue & mask) & (C_MAJOR_SCALE_MASK & mask)) == mask)
            {
                if(thisBar[i].beatLength > CROTCHET)
                    currentFitness--;
            }
        }
    }
    
    //__________reward c major chord notes__________(NOT WIGGINS)//
    for(int i = 0; i < thisBar.size(); i++)
    {
        //for every bit of our beats value
        for(int x = 1; x <= 13; ++x)
        {
            int mask = 1 << (x-1); //1 to the power of current bit being checked
            
            //if this bar is in the cmajor scale...
            if(((thisBar[i].beatValue & mask) & (C_MAJOR_TRIAD_MASK & mask)) == mask)
            {
                //give a point cause of generosity...
                currentFitness++;
            }
        }

    }
    DataLogging::writeToDataLog((String)currentFitness);
    std::cout<< currentFitness << std::endl;
    return currentFitness;

}



