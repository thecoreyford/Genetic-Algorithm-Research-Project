/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

/*
 ==============================================================================
 
 REFERENCES:
    -> Robinson, M., (2013) Getting started with JUCE. Packt Publishing Ltd.
    -> JUCE. 2018. Tutorial: Build a sine wave synthesiser. [ONLINE] Available at: https://docs.juce.com/master/tutorial_sine_synth.html. [Accessed 10 April 2018].
 
 ==============================================================================
 */

#include "../JuceLibraryCode/JuceHeader.h"

#include "CoreysComponents.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public AudioAppComponent, public Timer//, public MidiInputCallback
{
public:
    //==============================================================================
    MainContentComponent()
    {
        /*_______________________Init GUI___________________________*/
        setSize (1000, 700); //fixed window (non-resizeable)
        mainRect = getLocalBounds(); //capture size in a rectangle
        
        
        //start the timer to check what screen is being used
        startTimer(100);//check screen every 100 milliseconds
        screenCounter = 0; //start with the first screen
        /*__________________________________________________________*/
        
        // specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }

    ~MainContentComponent()
    {
        shutdownAudio(); //stops any audio sound being currently process on
                         // programme closure
    }

    //==============================================================================
    
    /*_____________________________AUDIO PROCESSING_______________________________*/
    /*Note: this programme simply works and a midi-output device, therefore the sound
        during the research is synthesised using logic.
        See the MIDISequencePlayback class as part of the Genetic Algorithm group to see
            how this is done.
     */
    
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override{}

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        // Right now we are not producing any data, in which case we need to clear the buffer
        // (to prevent the output of random noise) or microphone input
        bufferToFill.clearActiveBufferRegion();
    }

    void releaseResources() override{}
    /*____________________________________________________________________________*/
    
    //==============================================================================
    
    void paint (Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (Colours::black);
    }
    
    //===============================================================================
    
    /**
     Enumeration for screen indicies.
     */
    enum {
        EXPLAIN_SCREEN = 0, GA_SCREEN = 1, PLAYBACK_SCREEN = 2
    };

    /**
     Checks which screen should be visible and sets that accordingly.
     */
    void timerCallback() override
    {
        //find out what screen we are currently on
        switch(screenCounter)
        {
            case EXPLAIN_SCREEN:
                screenCounter = screen0.screenCount();
                break;
            case GA_SCREEN:
                screenCounter = screen1.screenCount();
                break;
            case PLAYBACK_SCREEN:
                break;
        }
        
        //set visibility accordingly
        switch(screenCounter)
        {
            case EXPLAIN_SCREEN:
                screen0.setBounds(mainRect);
                addAndMakeVisible(&screen0);
                break;
            case GA_SCREEN:
                screen1.setBounds(mainRect);
                addAndMakeVisible(&screen1);
                break;
            case PLAYBACK_SCREEN:
                screen2.setBounds(mainRect);
                addAndMakeVisible(&screen2);
                break;
        }
    }

private:
    //==============================================================================

    // Your private member variables go here...
    Rectangle<int> mainRect;
    
    //Screens
    int screenCounter;
    ExplanationScreen screen0;
    GAScreen screen1;
    PlaybackScreen screen2;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }
