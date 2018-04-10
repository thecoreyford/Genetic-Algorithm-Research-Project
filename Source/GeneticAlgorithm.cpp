/*
 ==============================================================================
 
 GeneticAlgorithm.cpp
 Created: 9 Mar 2018 3:21:52pm (work done of this class dosen't change any
 of the algorithms tested during conducting
 the research sesssion,  but simply tidyed
 the project.)
 Author:  Corey Ford
 
 ==============================================================================
 */

#include "GeneticAlgorithm.h"

GeneticAlgorithm::GeneticAlgorithm()
{
    //start on the 1st gene & 1st generation
    geneNumber = 0;
    generationNumber = 0;
    
    //set the fitness of every gene in the population to start at zero
    for(int i = 0; i < POPULATION_SIZE; i++)
    {
        pool[i].fitness = 0;
        startPool[i] = pool[i]; //make a copy of the starting pool for the ruleBasedGA to compare with
    }
    
    //make this object an action listener for the broadcasts
    sequencePlayBack.addActionListener(this);
}

GeneticAlgorithm::~GeneticAlgorithm(){}

//==========================================================================

void GeneticAlgorithm::actionListenerCallback (const String& message)
{
    if(message == "FINISHED_PLAYBACK") //of one gene
    {
        geneNumber++; //increment onto the next gene
        doIGA();
    }
}

void GeneticAlgorithm::doIGA()
{
    if(generationNumber != NO_OF_GENERATIONS)
    {
        if(geneNumber != POPULATION_SIZE)
        {
            if(geneNumber == 0) //at the start of our iteration
            {
                mutation(); //..mutate the population
                DataLogging::writeToDataLog("====GENERATION " + (String)generationNumber + " ====");
                std::cout << "====GENERATION " << generationNumber << "====" << std::endl;//print the current generation
            }
            
            //get the array of midi messages for this gene
            Array<MidiMessage> capturedMessage = manipulator.getBeatsAsMIDIMessage(pool[geneNumber].population.getBeatArray());
            sequencePlayBack.start(capturedMessage);
        }
        else
        {
            //store the generation into a file
            manipulator.sequenceToMIDIFile(getAndResetMessageBundle(), "Interactive -" + (String) generationNumber );
            
            generationNumber++; //increment generation
            reproduce(); //reproduce in preperaton for the next generation
            geneNumber = 0; //reset gene number to zero
            doIGA();
        }
    }
    else
    {
        //once we have reached the end of the generation, lets use the rule based GA...
        DataLogging::writeToDataLog("******************RULE BASED GA******************");
        doRuleBasedGA();
        ActionBroadcaster::sendActionMessage(ENABLE_CONTINUE); //trigger that we have finished playback
    }
}

void GeneticAlgorithm::doRuleBasedGA()
{
    //re-initialise these values
    generationNumber = 0;
    geneNumber = 0;
    
    for(int i =0; i < POPULATION_SIZE; i++)
        pool[i] = startPool[i]; //make pool the original set of data
    
    for(int i =0; i < NO_OF_GENERATIONS; i++)
    {
        generationNumber = i;
        mutation();
        DataLogging::writeToDataLog("====GENERATION " + (String)i + " ====");
        std::cout << "====GENERATION " << i << "====" << std::endl;
        //trigger midi messages?
        manipulator.sequenceToMIDIFile(getAndResetMessageBundle(), "RuleBased -" + (String) generationNumber );
        applyRuleBasedFitness();
        reproduce();
    }
}
//==========================================================================

void GeneticAlgorithm::mutation()
{
    int mutationChance = 0;
    //for every gene in the population...
    for(int i = 0; i < POPULATION_SIZE; i++)
    {
        mutationChance = arc4random_uniform(MIDIBar::TOTAL_MUTATIONS + 6); //5 out of the number passed
        if(mutationChance < MIDIBar::TOTAL_MUTATIONS)
        {
            DataLogging::writeToDataLog("--> Mutation " + (String)mutationChance + "occoured for bar " + (String) i );
            std::cout<< "--> Mutation " << mutationChance << "occoured for bar " << i << std::endl;
        }
        //mutate bar depending on this chance of mutation
        pool[i].population.mutate(mutationChance);
    }
}

//==========================================================================
Array<Array<MidiMessage>> GeneticAlgorithm::getAndResetMessageBundle()
{
    geneNumber = 0; //start on the zeroth gene
    
    Array<Array<MidiMessage>> bundleOfMessages; //create a bundle of messages
    bundleOfMessages.clear(); //ensure this is cleared
    
    //for every gene in the population...
    for(int i = 0; i < POPULATION_SIZE; i++)
    {
        geneNumber = 0;
        //grab and store the messages to be played back
        Array<MidiMessage> capturedMessage = manipulator.getBeatsAsMIDIMessage(pool[i].population.getBeatArray());
        bundleOfMessages.add(capturedMessage); //add these to our bundle
    }
    
    return bundleOfMessages;
}


//==============================================================================

void GeneticAlgorithm::applyInteractiveFitness(int change)
{
    DataLogging::writeToDataLog("Applied " + (String)change + " to gene number " + (String)geneNumber);
    std::cout << "Applied " << change << " to gene number " << geneNumber << std::endl;
    pool[geneNumber].fitness += change;
}

void GeneticAlgorithm::applyRuleBasedFitness()
{
    for(int i = 1; i < (POPULATION_SIZE+1); i++)
    {
        pool[i].fitness = pool[i].population.selfAssess(pool[i-1].population.getBeatArray());
    }
}

//==============================================================================

void GeneticAlgorithm::reproduce()
{
    //store the index of each gene within the pool in decremental order...
    //...of fitness (highest fitness first)
    int fittestIndex[POPULATION_SIZE] = {0};
    
    //intialise it with a current 0-POPULATION_SIZE order
    for(int i = 0 ; i < POPULATION_SIZE; i++)
        fittestIndex[i] = i;
    
    /*________________________________________________________________________*/
    /*Basic bubble sort.. does the job!!! (like in the midi manipulator class)*/
    bool swaps = true;
    while(swaps == true)
    {
        //assume their are no swaps
        swaps = false;
        //for each value
        for(int i = 1; i <= (POPULATION_SIZE - 1); i++)
        {
            int indexA = fittestIndex[i-1];
            int indexB = fittestIndex[i];
            //compare the fitness
            if(pool[indexA].fitness < pool[indexB].fitness)
            {
                //swap these for the index array
                int temp = 0;
                temp = fittestIndex[i-1];
                fittestIndex[i-1] = fittestIndex[i];
                fittestIndex[i] = temp;
                
                //alter that swaps must now be true
                swaps = true;
            }
        }
    }
    /*________________________________________________________________________*/
    
    /*we will replace 50% of the pool by replacing the final 4 stored inices
     with a random combination of the 1st four "fittest"*/
    int fittestA = 0;
    int fittestB = 0;
    
    for(int i = 4; i < POPULATION_SIZE; i++)
    {
        //pick two of the fittest indexes
        fittestA = fittestIndex[arc4random_uniform(4)]; //creates a number from 0-3
        fittestB = fittestIndex[arc4random_uniform(4)]; //creates a number from 0-3
        
        //store these "beat array"
        Array<Beat> parentA = pool[fittestA].population.getBeatArray();
        Array<Beat> parentB = pool[fittestB].population.getBeatArray();
        
        //perform crossover
        pool[i].population.crossover(parentA, parentB);
    }
    
    //replace a random bar with a new generated bar to maintain variation sometimes?
    int randomChance = arc4random_uniform(2);
    if(randomChance ==0)
    {
        int whichBar = arc4random_uniform(8); //one of the bars
        MIDIBar variationBar;
        pool[whichBar].population = variationBar;
    }
    
    //reset all fitness to zero for the next generation
    for(int i = 0; i < POPULATION_SIZE; i++)
        pool[i].fitness = 0;
}

//==========================================================================

