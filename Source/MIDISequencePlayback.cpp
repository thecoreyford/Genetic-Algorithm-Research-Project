/*
 ==============================================================================
 
 MIDISequencePlayback.cpp
 Created: 22 Feb 2018 11:31:12am
 Author:  Corey Ford
 
 ==============================================================================
 */

#include "MIDISequencePlayback.h"

MIDISequencePlayback::MIDISequencePlayback()
{
    messages.clear(); //ensure array of messages are clear
    startTimer(1);//start a timer every milisecond
    
    masterMidiOutput = MidiOutput::createNewDevice("midiOut"); //make this a midi output device
    
}

MIDISequencePlayback::~MIDISequencePlayback()
{
    masterMidiOutput->clearAllPendingMessages();
    delete masterMidiOutput;
}

void MIDISequencePlayback::start(Array<MidiMessage> playMe)
{
    messages = playMe; //transfer messages into this class
    playState = 1; //say yes we want to start playing
    playPosition = 0; //start at the begginging of the array of messages
    timeStart = Time::getMillisecondCounterHiRes(); //get the time start
}

void MIDISequencePlayback::timerCallback()
{
    if(playState == 1)
    {
        //figure out how much time has elapsed
        double elapsedTime = Time::getMillisecondCounterHiRes() - timeStart;
        
        //output this message when appropriate
        if(elapsedTime > messages[playPosition].getTimeStamp())
        {
            //output this message
            masterMidiOutput->sendMessageNow(messages[playPosition]);
            
            //increment through the array
            playPosition++;
        }
        
        //if we have reached the end of the array
        if(playPosition > messages.size())
        {
            playState = 0; //turn off playback for this message
            ActionBroadcaster::sendActionMessage(FINISHED_PLAYBACK); //trigger that we have finished playback
        }
    }
}
