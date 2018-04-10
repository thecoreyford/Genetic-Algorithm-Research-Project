/*
  ==============================================================================

    PlaybackScreen.h
    Created: 3 Feb 2018 4:15:24pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================

class PlaybackScreen    : public Component
{
public:
    /**
     Constructor, does nothing.
     */
    PlaybackScreen()
    {
        /*As this is the last screen we don't need to check for activity, 
         we can presume that it is active until program is closed.*/
    }

    /**Destructor, does nothing.*/
    ~PlaybackScreen(){}

    /**Paints elements onto screen */
    void paint (Graphics& g) override
    {
        
        g.fillAll (Colours::lightyellow);   // clear the background
        
        g.setColour (Colours::black);
        
        g.setFont (20.0f);
        g.drawText ("Thank you for participating. Please ask your interviewer for the final questions.", getLocalBounds(),
                    Justification::centred, true);   // draw some placeholder text
    }

    void resized() override{}
    
    //==========================================================================

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaybackScreen)
};
