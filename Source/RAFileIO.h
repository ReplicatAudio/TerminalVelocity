/*
  ==============================================================================

    FilIO.h
    Created: 4 Nov 2022 11:59:57pm
    Author:  dombr
    v3
  ==============================================================================
*/

#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <JuceHeader.h>
class RAFileIO
{
public:
    RAFileIO(juce::String newPath = "__na__")
    {
        if (newPath == "__na__")
        {
            juce::String productName = ProjectInfo::projectName;
            newPath = productName;
        }
        pathRA = juce::File::getSpecialLocation(juce::File::SpecialLocationType::userHomeDirectory).getChildFile("ReplicatAudio");
        pathApp = pathRA.getChildFile(newPath);
    }
    ~RAFileIO() {};
    // Not OS agnostic only for end-users
    void setBasePath(juce::String newPath) 
    {
        pathApp = juce::File(newPath);
    }
    juce::String getPathRAString() 
    {
        return pathRA.getFullPathName();
    }
    juce::File getPathRA()
    {
        return pathRA;
    }
    bool pathRAExists()
    {
        if (pathRA.isDirectory())
        {
            return true;
        }
        return false;
    }
    bool pathExists(juce::String pathString)
    {
        if (juce::File(pathString).isDirectory())
        {
            return true;
        }
        return false;
    }
    // Checks if a file exists in the base path
    bool fileExists(juce::String pathString)
    {
        juce::File path(pathString);
        bool exists = path.existsAsFile();
        return exists;
    }
    // Switched filter to beginning add subdir
    juce::Array<juce::File> directoryListing(juce::String filter = "*", int type = 3, bool recursive = true)
    {
        juce::Array<juce::File> files = pathApp.findChildFiles(type, recursive, filter);
        return files;
    }
    // Switched filter to beginning add subdir
    juce::Array<juce::String> directoryListingStrings(juce::String filter = "*", int type = 3, bool recursive = true)
    {
        juce::Array<juce::File> files = pathApp.findChildFiles(type, recursive, filter);
        juce::Array<juce::String> fileNames;
        for (juce::File file : files)
        {
            fileNames.add(file.getFileNameWithoutExtension());
        }
        return fileNames;
    }
    void writeFile(juce::String fileName, juce::String content = "poop", juce::String subDir = "__na__") {
        juce::File path(pathApp.getChildFile(fileName));
        path.create();
        path.replaceWithText(content, false);
    }
    void writeFileRA(juce::String fileName, juce::String content)
    {
        juce::File path(pathRA.getChildFile(fileName));
        path.create();
        path.replaceWithText(content, false);
    }
    void writeFileAbs(juce::String filePath, juce::String fileName, juce::String content = "poop")
    {
        juce::File path(juce::File(filePath).getChildFile(fileName));
        path.create();
        path.replaceWithText(content, false);
    }
    void revealFile(juce::String fileName)
    {
        juce::File path(pathApp.getChildFile(fileName));
        path.revealToUser();
    }
    juce::String loadFile(juce::String fileName) {
        juce::File path(pathApp.getChildFile(fileName));
        if (!path.existsAsFile())
            return "na";
        auto fileText = path.loadFileAsString();
        return fileText;
    }
    juce::String loadFileRA(juce::String fileName) {
        juce::File path(pathRA.getChildFile(fileName));
        if (!path.existsAsFile())
            return "na";
        auto fileText = path.loadFileAsString();
        return fileText;
    }
    juce::String loadFileAbs(juce::String fileName) {
        juce::File path(fileName);
        if (!path.existsAsFile())
            return "No File";
        auto fileText = path.loadFileAsString();
        return fileText;
    }
private:
    juce::File pathRA;
    juce::File pathApp;
};
