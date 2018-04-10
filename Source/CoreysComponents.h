/*
  ==============================================================================

    CoreysComponents.h
    Created: 29 Jan 2018 4:10:00pm
    Author:  Corey Ford

  ==============================================================================
*/

/*
 ==============================================================================
 
 NOTES:
    -> Each class is categorised and placed into groups.
 
    -> All the ones noted here are written by me with the exception of those 
        generated (Main, & Main Component). However, anyone comfortable with JUCE
        will know what code I have added to the wrapper.
 
    -> References regarding the learning of the code are placed in the header for 
        each applicable class.
 
    -> Enjoy :)
 
 ==============================================================================
 */

#pragma once

//Data Logging (classes related to saving data)
#include "DataLogging.h"

//MIDI creation (classes related to creating midi files / data)
#include "MIDIBar.h"
#include "MIDIManipulator.h"

//Screens (GUI elements)
#include "ExplanationScreen.h"
#include "GAScreen.h"
#include "PlaybackScreen.h"

//Genetic Algorithm (the machine learning stuff & realtime playback classes [for IGA])
#include "MIDISequencePlayback.h"
#include "GeneticAlgorithm.h"

