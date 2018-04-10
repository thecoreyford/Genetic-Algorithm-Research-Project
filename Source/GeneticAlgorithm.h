/*
 ==============================================================================
 
 GeneticAlgorithm.h
 Created: 16 Feb 2018 7:47:42pm
 Author:  Corey Ford
 
 ==============================================================================
 */
/*
 ==============================================================================
 
 REFERENCES:
 -> JUCE. 2018. ActionListener Class Reference. [ONLINE] Available at: https://docs.juce.com/master/classActionListener.html. [Accessed 10 April 2018]. 
 
 ==============================================================================
 */

#pragma once

#include "MIDIBar.h"
#include "MIDISequencePlayback.h"
#include "MIDIManipulator.h"

#define POPULATION_SIZE 8
#define NO_OF_GENERATIONS 15


//==============================================================================

struct Gene {
    MIDIBar population;
    int fitness;
};

//==============================================================================

class GeneticAlgorithm: public ActionListener, public ActionBroadcaster{
public:
    
    /**
     Constructor. Intialises values to their start values, and sets this up as 
     an action listener.
     */
    GeneticAlgorithm();
    
    /**
     Destructor, does nothing.
     */
    ~GeneticAlgorithm();
    
    //==========================================================================
    
    /**
     Callback for the action listener. Detects when a message has finished and 
     increments foward to the next gene.
     */
    void actionListenerCallback (const String & message);

    //Used for the broadcaster, saying when the continue button should show.
    const char * ENABLE_CONTINUE = "ENABLE_CONTINUE";

    /**
     Cycles playback for the population, and recussivley calls itself one ready
     for the next generation.
     */
    void doIGA();
    
    /**
     Iterativley starts the GA with a different fitness fucntion to doIGA, 
     starting with the same initial set of genes.
     */
    void doRuleBasedGA();
    //==========================================================================
    
    /**
     Randomly decides if a mutation should occour.
     */
    void mutation();
    
    //==========================================================================
    
    /**
     Get a bundle of messages to be converted as a MIDI file for data logging.
     */
    Array<Array<MidiMessage>> getAndResetMessageBundle();
    
    //==========================================================================
    /**
     Adds or removes fitness based on the input to the fitness value 
     for the current playing gene.
     */
    void applyInteractiveFitness(int change);
    
    /**
     Calls for each gene to be assesed and assignes it's fitness value.
     */
    void applyRuleBasedFitness();
    
    //==========================================================================
    
    /**
     Creates a 50% crossover for the population based on the fitness values.
     */
    void reproduce();
    
    //==========================================================================
    
private:
    Gene pool[POPULATION_SIZE], startPool[POPULATION_SIZE];
    MIDIManipulator manipulator;
    int geneNumber, generationNumber;
    MIDISequencePlayback sequencePlayBack;
};
