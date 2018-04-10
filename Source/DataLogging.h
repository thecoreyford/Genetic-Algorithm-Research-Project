/*
  ==============================================================================

    DataLogging.h
    Created: 25 Feb 2018 1:42:41pm
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

//define the static variable
//-> any advice on how to store something like the folder path pernamently?
static String FOLDER_PATH;
static const String DATA_PATH = "./programData.txt";
static const String LOG_NAME = "/dataLog.txt";

class DataLogging
{
public:
    /**
     Creates a folder dirctory based on the ID input
     */
    static void setupDirectory(String ID)
    {
        FOLDER_PATH = "./" + ID;
        
        //create a new folder
        File folder(FOLDER_PATH);
        if(folder.isDirectory() == false)
            folder.createDirectory();
        
        initialiseDataLogTextFile(ID);
    }
    
    /**
     Writes the input string to the dataLog file from the path stored within 
     the stored path.
     */
    static void writeToDataLog(String messageToAppend)
    {
        FOLDER_PATH = getStoredFolderPath();
        
        File file (FOLDER_PATH + LOG_NAME);
        file.appendText("\n");
        file.appendText(messageToAppend);
    }
    
    /**
     Returns the path stored within the program data text file.
     */
    static String getStoredFolderPath()
    {
        File pathLogger(DATA_PATH);
        return "./" + pathLogger.loadFileAsString();
    }
    
private:
    /**
     Creates a data log within the folder based on the ID passed
     */
    static void initialiseDataLogTextFile(String ID)
    {
        File pathLogger(DATA_PATH);
        pathLogger.replaceWithText(ID);
        
        File dataLog(FOLDER_PATH + LOG_NAME);
        dataLog.replaceWithText("Participant ID ** " + ID);
    }
};
