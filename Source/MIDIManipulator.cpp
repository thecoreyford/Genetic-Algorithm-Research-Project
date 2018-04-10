/*
  ==============================================================================

    MIDIManipulator.cpp
    Created: 9 Feb 2018 5:53:21pm
    Author:  Corey Ford

  ==============================================================================
*/

#include "MIDIManipulator.h"

#define CONST_VELOCITY (uint8) 88
#define TICKS_PER_CROTCHET 500.0 //--> using 500 so each tick is quater a second long

//==============================================================================

MIDIManipulator::MIDIManipulator(){}

//==============================================================================

MIDIManipulator::~MIDIManipulator(){}

//==============================================================================

Array<MidiMessage> MIDIManipulator::getBeatsAsMIDIMessage(Array<Beat> thisBar)
{
    //setup variables
    double timestamp = 0.0;
    Array<MidiMessage> thisBarMIDI; //create an array of midimessages
    thisBarMIDI.clear(); //ensure array is empty before filling it.
    
    //for every beat in the bar (each array element)
    for(int i = 0; i < thisBar.size(); ++i)
    {
        //for every bit of our beats value
        for(int x = 1; x <= 13; ++x)
        {
            int mask = 1 << (x-1); //1 to the power of current bit being checked
            
            //if 'x' bit is 1 (bit masking)...
            if((thisBar[i].beatValue & mask) == mask)
            {
                MidiMessage messageOn, messageOff;
                
                //create the note on within limts
                int noteValue = (x + 59 + thisBar[i].increment);
                jassert(noteValue >= 0);
                
                //using midi channel 2...
                messageOn = MidiMessage::noteOn(2, (noteValue), CONST_VELOCITY); //create a midiOn
                messageOn.setTimeStamp(timestampToMidiTicks(timestamp));//set timestamp
                
                messageOff = MidiMessage::noteOff(2, (noteValue));//create a midiOff
                messageOff.setTimeStamp(timestampToMidiTicks(timestamp + (double) thisBar[i].beatLength));//set timestamp
                
                //push these messages onto the MIDI message
                thisBarMIDI.add(messageOn);
                thisBarMIDI.add(messageOff);
            }
        }
        
        //increment timestamp ready for next beat...
        timestamp += (double) thisBar[i].beatLength;
    }
    
    return sort(thisBarMIDI); //sort for the MIDI messages, and return
}

//==============================================================================

Array<MidiMessage> MIDIManipulator::sort(Array<MidiMessage> thisBarMIDI)
{
    /*Basic bubble sort.. does the job!!!*/
    bool swaps = true;
    while(swaps == true)
    {
        //assume their are no swaps
        swaps = false;
        //for each value
        for(int i = 1; i <= (thisBarMIDI.size() - 1); i++)
        {
            //compare the timestamp order
            if(thisBarMIDI[i-1].getTimeStamp() > thisBarMIDI[i].getTimeStamp())
            {
                //swap these if not in order
                thisBarMIDI.swap(i-1, i);
                //alter that swaps must now be true
                swaps = true;
            }
        }
    }
    
    return thisBarMIDI;
}

//==============================================================================

double MIDIManipulator::timestampToMidiTicks(double timestamp)
{
    timestamp /= (double) MIDIBar::CROTCHET;
    timestamp *= TICKS_PER_CROTCHET;
    return timestamp;
}

//==============================================================================

void MIDIManipulator::sequenceToMIDIFile(Array<Array<MidiMessage>> bundle, String filename)
{
    /*__________create a midi message sequence from the input bundle__________*/
    MidiMessageSequence thisSequence; //create a midi message sequence
    thisSequence.clear(); //ensure its empty
    for(int gene = 0; gene < bundle.size(); gene++) //for each gene...
    {
        Array<MidiMessage> notes = bundle[gene];
        for(int note = 0; note < notes.size(); note++)
        {
            thisSequence.addEvent(notes[note]); //...add it to to the sequence!
        }
    }
    thisSequence.sort(); //sort timecodes
    
    
    //create a midi file from the input sequence...
    MidiFile midiFile;
    midiFile.clear();//force clear before any alterations
    midiFile.setTicksPerQuarterNote(TICKS_PER_CROTCHET);
    midiFile.addTrack(thisSequence);//place sequence in midi file
    
    //place into files
    String folderPath = DataLogging::getStoredFolderPath();
    File fileToWrite(folderPath + "/" +filename+".mid");
    ScopedPointer <FileOutputStream> fileOutput(fileToWrite.createOutputStream());
    midiFile.writeTo(*fileOutput);
}
