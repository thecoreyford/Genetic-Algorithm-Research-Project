/*
 ==============================================================================
 
 MIDIBar.h
 Created: 1 Jan 2018 10:11:19pm
 Author:  Corey Ford
 
 ==============================================================================
 */

/*
 ==============================================================================
 
 REFERENCES:
    -> JUCE. 2018. MidiMessageSequence Class Reference. [ONLINE] Available at: https://docs.juce.com/master/classMidiMessageSequence.html. [Accessed 10 April 2018].
    -> JUCE. 2018. Tutorial: Handling MIDI events. [ONLINE] Available at: https://docs.juce.com/master/tutorial_handling_midi_events.html. [Accessed 10 April 2018].
 
 ==============================================================================
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================

/**
 Data structure for any note/chord (beat) appearing in the MIDI bar.
 */
struct Beat
{
    signed int beatValue;
    signed int beatLength;
    signed int increment;
};

//==============================================================================

class MIDIBar
{
public:
    /**
     Constuctor Method Randomly determins a bars length worth of beats to populate array.
     */
    MIDIBar();
    
    /**
     Destructor method. Currently does nothing.
     */
    ~MIDIBar();
    
    //==========================================================================
    
    /**
     Enumeration for note value lengths in milliseconds at 60 bpm (approx)
     */
    enum{
        SEMIBREVE = 4000, MINIM = 2000, CROTCHET = 1000, DOTTED_CROTCHET = 1500, QUAVER = 500, DEMI_SEMI_QUAVER = 100
    };
    
    //==========================================================================
    
    /**
     Returns the array of beats for this object
     */
    Array<Beat> getBeatArray(){ return thisBar; }
    
    //==========================================================================
                        /* The functions below are used for
                            manipulating the GA - for mutating
                                or reproducing*/
    //==========================================================================
    
    /**
     Enumeration for mutation descriptions
     */
    enum{
        cMajMask, toneShift, segmentRepeat, increment, TOTAL_MUTATIONS
    };
    
    /**
     Applies a mutation based on the numeric value input to this bar
     */
    void mutate(int selection);
    
    /**
     Replaces this bars contents with
     */
    void crossover(Array<Beat> parentA, Array<Beat> parentB);
    
    /**
     Returns a total calculated fitness value for this bar
     */
    int selfAssess(Array<Beat> previousBar);
    
private:
    Array<Beat> thisBar; //store messages as an array of beats
};

