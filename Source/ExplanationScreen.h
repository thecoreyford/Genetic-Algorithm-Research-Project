/*
 ==============================================================================
 
 ExplanationScreen.h
 Created: 29 Jan 2018 4:06:39pm
 Author:  Corey Ford
 
 ==============================================================================
 */

/*
 ==============================================================================
 
 REFERENCES:
 -> Robinson, M., (2013) Getting started with JUCE. Packt Publishing Ltd.
 
 ==============================================================================
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#define NO_OF_LINES 4

//==============================================================================

class ExplanationScreen    : public Component, public Button::Listener
{
public:
    /**
     Constructor. Sets button listeners object creation (before bounds are set)
     */
    ExplanationScreen()
    {
        //set this page as currently active
        active = true;
        
        //setup button
        submitButton.setButtonText("Next...");
        addAndMakeVisible(submitButton);
        submitButton.addListener(this);
        
        //setup textbox
        addAndMakeVisible(IDTextBox);
    }
    
    /**Destructor. Currently does nothing.*/
    ~ExplanationScreen(){}
    
    /**
     Draws all elements onto the screen.
     */
    void paint (Graphics& g) override
    {
        //create rectangles to divide GUI sections into.
        Rectangle<int> all, top, middleTop, middleBottom[NO_OF_LINES + 1], bottom;

        //make all rectangles equal the whole screen
        all = top = middleTop = bottom = getLocalBounds();
        
        //remove sections unwanted for that rectange...
        top.removeFromBottom(all.getHeight()*0.75);
        
        middleTop.removeFromBottom(all.getHeight()*0.5);
        middleTop.removeFromTop(all.getHeight()*0.25);
        
        middleBottom[0] = all;
        middleBottom[0].removeFromTop(all.getHeight()*0.5);
        middleBottom[0].removeFromBottom(all.getHeight()*0.25);
        for(int i = 1; i <= NO_OF_LINES; ++i) //and do so for each line in the middle bottom section
        {
            middleBottom[i] = middleBottom[0];
            middleBottom[i].removeFromBottom(middleBottom[0].getHeight()*(1.0 - (i*0.25)));
        }
        
        bottom.removeFromTop(all.getHeight()*0.75);
        
        /*____________________________________________________________________*/
        //draw in text and GUI elements...
        
        g.fillAll (Colours::lightyellow);
        g.setColour (Colours::black);
        
        g.drawText ("Welcome to Corey's Research project!", top,
                    Justification::centred, true);
        
        g.drawText ("Please enter your anonymous ID from the top of your questionaire here ---------->", middleTop,
                    Justification::left, true);
    
        IDTextBox.setBounds((middleTop.getWidth() * 0.5), middleTop.getY(), (middleTop.getWidth() * 0.5), middleTop.getHeight());
        
        g.drawText ("Please complete page one of the questionare before continuing.", middleBottom[0], Justification::centred, true);
        g.drawText ("On clicking next you will hear a piece of music.", middleBottom[1], Justification::centred, true);
        g.drawText ("Press 'g' when you hear something you like, and press 'b' when you hear something you don't.", middleBottom[2], Justification::centred, true);
        g.drawText ("As the music progresses it should start to sound stronger.", middleBottom[3], Justification::centred, true);
        
        submitButton.setBounds(bottom);
    }
    
    void resized() override{}
    
    //==========================================================================
    
    /** 
     Called when the button is clicked. 
     */
    virtual void buttonClicked (Button* button) override
    {
        DataLogging::setupDirectory(IDTextBox.getText());
        //once file is written set the introduction screen to inactive
        active =  false;
    }
    
    //==========================================================================
    /**
     Sends to the timer within main to modify the screen number being viewed.
     */
    int screenCount()
    {
        if(!active)
            this->setVisible(false);
        
        return !active;
    }
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ExplanationScreen)
    bool active;
    TextButton submitButton;
    TextEditor IDTextBox;
};
