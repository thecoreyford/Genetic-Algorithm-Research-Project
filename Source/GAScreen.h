/*
  ==============================================================================

    GAScreen.h
    Created: 3 Feb 2018 10:32:12am
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "GeneticAlgorithm.h"

//==============================================================================
/*
*/
class GAScreen : public Component, public Button::Listener, public KeyListener, public ActionListener
{
public:
    /**
     Constructor. Starts listener for this screen.
     */
    GAScreen()
    {
        //set this page as currently active
        active = true;
        
        //setup continue button (will appear over time eventually)
        continueButton.setButtonText("continue...");
        continueButton.addListener(this);
        
        //setup start button
        startButton.setButtonText("Start IGA");
        addAndMakeVisible(startButton);
        startButton.addListener(this);
        
        //setup textEditor
        addAndMakeVisible(logBox);
        logBox.setMultiLine (true);
        logBox.setReturnKeyStartsNewLine (true);
        logBox.setReadOnly (true);
        logBox.setScrollbarsShown (true);
        logBox.setCaretVisible (false);
        logBox.setPopupMenuEnabled (true);
        
        //include key listener
        addKeyListener(this);
        setWantsKeyboardFocus(true);
        hasKeyboardFocus(true);
        
        //include action listener
        GA.addActionListener(this);
    }

    /**Destructor, does nothing!*/
    ~GAScreen(){}

    /**Draws elements onto the screen*/
    void paint (Graphics& g) override
    {
        //divide screen into segments
        Rectangle<int> all = getLocalBounds();
        Rectangle<int> bottom, left, right, top;
        bottom = left = right = top = all;
        bottom.removeFromTop(all.getHeight() * 0.75);
        left.removeFromRight(all.getWidth() * 0.5);
        left.removeFromBottom(bottom.getHeight());
        right.removeFromLeft(all.getWidth() * 0.5);
        right.removeFromBottom(bottom.getHeight());
        top.removeFromBottom(all.getHeight()*0.25);

        //provide instructions and shade bottom half
        g.setFont (20.0f);
        g.setColour (Colours::lightyellow);
        g.fillRect(bottom);
        g.setColour(Colours::black);
        g.drawText ("Press 'g' for good bits, press 'b' for bad bits.", bottom, Justification::centred, true);
        
        //place log box above button display
        logBox.setBounds(top);
        
        //place buttons onto screen
        Rectangle<int> rightButtonSpace = bottom;
        rightButtonSpace.removeFromLeft(bottom.getWidth() * 0.75);
        continueButton.setBounds(rightButtonSpace);
        
        Rectangle<int> leftButtonSpace = bottom;
        leftButtonSpace.removeFromRight(bottom.getWidth() * 0.75);
        startButton.setBounds(leftButtonSpace);
    }

    void resized() override{}
    
    //==========================================================================
    /** 
     Called when a button is clicked. 
     */
    virtual void buttonClicked (Button* button) override
    {
        if(button == &continueButton)
        {
            //set this screen to be inactive, ready for the next screen
            active =  false;
        }
        
        if(button == &startButton)
        {
            //turn off this button
            startButton.setVisible(false);
            DataLogging::writeToDataLog("******************INTERATIVE GA******************");
            GA.doIGA();
        }
    }
    
    /**
     Action called to enable the continue button.
     */
    void actionListenerCallback (const String& message) override
    {
        if(message == "ENABLE_CONTINUE")
        {
            addAndMakeVisible(continueButton);
        }
    }
    
    //==========================================================================
    /**
     Adds the input message to the log box.
     */
    void logMessage(const String& m)
    {
        logBox.moveCaretToEnd();
        logBox.insertTextAtCaret (m + newLine);
    }
    
    //==========================================================================
    
    /**
     Detects key press events and updates fitness for the current bar accordingly
     */
    bool keyPressed(const KeyPress &key, Component* originatingComponent) override
    {
        if(key == KeyPress::createFromDescription("g") && !startButton.isVisible())
        {
            GA.applyInteractiveFitness(+1);
            logMessage("Good!!!");
        }
        
        if(key == KeyPress::createFromDescription("b") && !startButton.isVisible())
        {
            GA.applyInteractiveFitness(-1);
            logMessage("Bad!!!");
        }
        
        return true;
    }
    
    //==========================================================================
    /**
     Sends to the timer within main to modify the screen number being viewed.
     */
    int screenCount()
    {
        //when inactive turn this screen off...
        if(!active)
            this->setVisible(false);
        
        //return the index for the next screen
        return (!active + 1);
    }
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GAScreen)
    bool active;
    TextButton continueButton, startButton;
    TextEditor logBox;
    GeneticAlgorithm GA;
};
