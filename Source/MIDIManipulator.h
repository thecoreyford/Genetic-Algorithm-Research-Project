/*
  ==============================================================================

    MIDIManipulator.h
    Created: 9 Feb 2018 5:53:21pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "MIDIBar.h"
#include "DataLogging.h"

class MIDIManipulator{
public:
    
    /**
     Creates a midi output for this object.
     */
    MIDIManipulator();
    
    //==========================================================================
    
    /**
     Delete the midi output.
     */
    ~MIDIManipulator();
    
    //==========================================================================
    
    /**
     Returns the array of midi messages for this object
     */
    Array<MidiMessage> getBeatsAsMIDIMessage(Array<Beat> thisBar);
    
    //==========================================================================
    
    /**
     Returns the sorted timecodes for the input midi message
     */
    Array<MidiMessage> sort(Array<MidiMessage> thisBarMIDI);
    
    //==========================================================================
    
    /**
     Converts an input timestamp in miliseconds to a midi tick value
     */
    double timestampToMidiTicks(double timestamp);
    
    //==========================================================================
    
    /**
     Writes the input midi message to a midi file.
     */
    
    void sequenceToMIDIFile(Array<Array<MidiMessage>> bundle, String filename);
private:
    /*Nothing*/
};
