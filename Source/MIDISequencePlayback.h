/*
 ==============================================================================
 
 MIDISequencePlayback.h
 Created: 22 Feb 2018 11:31:12am
 Author:  Corey Ford
 
 ==============================================================================
 */

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

/*
 ==============================================================================
 
 REFERENCES:
 -> Corey Ford. 2018. Lowering CPU on MIDI playback thread. [ONLINE] Available at: https://stackoverflow.com/questions/48907796/lowering-cpu-on-midi-playback-thread/48927880#48927880. [Accessed 10 April 2018].
 -> Hunt, S. (2018) E-mail to Corey Ford , 13 February.
 ==============================================================================
 */

class MIDISequencePlayback: public Timer, public ActionBroadcaster
{
public:
    
    //public string used to trigger the action broadcaster
    const char * FINISHED_PLAYBACK = "FINISHED_PLAYBACK";
    
    /**
     Constructor. Setups timer and defines this as a midi output device.
     */
    MIDISequencePlayback();
    
    /**
     Destructor. Deletes any output messages still within the buffer.
     */
    ~MIDISequencePlayback();
    
    /**
     Outputs messages at the correct point in time.
     */
    void timerCallback();
    
    /**
     Triggers the timer and initialised the time/playback tracking variables.
     */
    void start(Array<MidiMessage> playMe);
    
private:
    int playState, playPosition;
    MidiOutput * masterMidiOutput; //setup this class as a "midi output device"
    Array<MidiMessage> messages;   //store our message for playback
    double timeStart;
    
};
