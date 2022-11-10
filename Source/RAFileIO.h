/*
  ==============================================================================

    FilIO.h
    Created: 4 Nov 2022 11:59:57pm
    Author:  dombr
    v2
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
    RAFileIO(juce::String newPath)
    {
        basePath = newPath;
        //writeFile();
    }
    ~RAFileIO() {};
    void setBasePath(juce::String newPath) {
        basePath = newPath;
    }
    juce::String getBasePath() {
        return basePath;
    }
    bool basePathExists()
    {
        if (juce::File(basePath).isDirectory())
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
        juce::File path(juce::File(basePath).getChildFile(pathString));
        bool exists = path.existsAsFile();
        return exists;
    }
    bool testWrite()
    {
        juce::File path(juce::File(basePath).getChildFile("testFile"));
        path.create();
        bool exists = path.existsAsFile();
        path.deleteFile();
        return exists;
    }
    bool testRead()
    {
        juce::File path(juce::File(basePath).getChildFile("testFile"));
        path.create();
        path.replaceWithText("test content", false);
        juce::String testRead = path.loadFileAsString();
        path.deleteFile();
        if (testRead == "test content")
        {
            return true;
        }
        return false;
    }
    // Switched filter to beginning add subdir
    juce::Array<juce::File> directoryListing(juce::String filter = "*", juce::String subDir ="__na__", juce::String dir = "__base__", int type = 3, bool recursive = true)
    {

        if (dir == "__base__")
        {
            dir = basePath;
        }
        if (subDir != "__na__")
        {
            dir += "\\" + subDir;
        }
        jassert(pathExists(dir));
        juce::File path(dir);
        juce::Array<juce::File> files = path.findChildFiles(type, recursive, filter);
        return files;
    }
    // Switched filter to beginning add subdir
    juce::Array<juce::String> directoryListingStrings(juce::String filter = "*", juce::String subDir = "__na__", juce::String dir = "__base__", int type = 3, bool recursive = true)
    {

        if (dir == "__base__")
        {
            dir = basePath;
        }
        if (subDir != "__na__")
        {
            dir += "\\" + subDir;
        }
        jassert(pathExists(dir));
        juce::File path(dir);
        juce::Array<juce::File> files = path.findChildFiles(type, recursive, filter);
        juce::Array<juce::String> fileNames;
        for (juce::File file : files)
        {
            fileNames.add(file.getFileNameWithoutExtension());
        }
        return fileNames;
    }
    void writeFile(juce::String fileName, juce::String content = "poop", juce::String subDir = "__na__") {
        juce::String filePath = basePath;
        if (subDir != "__na__")
        {
             filePath += "\\" + subDir;
        }
        juce::File path(juce::File(filePath).getChildFile(fileName));
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
        juce::File path(juce::File(basePath).getChildFile(fileName));
        path.revealToUser();
    }
    juce::String loadFile(juce::String fileName, juce::String subDir = "__na__") {
        juce::String filePath = basePath;
        if (subDir != "__na__")
        {
            filePath += "\\" + subDir;
        }
        juce::File path(juce::File(filePath).getChildFile(fileName));
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
    juce::String basePath;
};
