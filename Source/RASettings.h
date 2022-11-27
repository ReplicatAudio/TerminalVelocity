/*
  ==============================================================================

    RASettings.h
    Created: 7 Nov 2022 4:13:17pm
    Author:  dombr
    v3
  ==============================================================================
*/

#pragma once
#include <map>
#include <JuceHeader.h>
#include "RAFileIO.h"
class RASettings
{
public:
    RASettings()
    {
        projectName = ProjectInfo::projectName;
    };
    ~RASettings() {};
    void load(RAFileIO fileIO)
    {
        //auto file = fileIO.loadFileAbs("C:\\ReplicatAudio\\"+projectName+".rasettings");
        auto file = fileIO.loadFileRA(projectName+".rasettings");
        auto valTree = juce::ValueTree::fromXml(file);
        for (const auto& pair : settingsMap) {
            auto first = pair.first;
            auto second = pair.second;
            if (valTree.hasProperty(first))
            {
                settingsMap[first] = valTree.getProperty(first);
            }
        }
    }
    void save(RAFileIO fileIO)
    {
        // Create the custom XML object representing the plugin state
        // Later the customer atrributes are loaded and manually set
        auto xmlSettings = juce::XmlElement("ReplicatAudioSettings");

        for (const auto& pair : settingsMap) {
            auto first = pair.first;
            auto second = pair.second;
            xmlSettings.setAttribute(first, second);
        }
        juce::String version = ProjectInfo::versionString;
        xmlSettings.setAttribute("version", version);

        auto xmlString = xmlSettings.toString();
        //fileIO.writeFileAbs("C:\\ReplicatAudio", projectName + ".rasettings", xmlString);
        fileIO.writeFileRA(projectName + ".rasettings", xmlString);
    }
    juce::String get(juce::String name)
    {
        return settingsMap.at(name);
    }
    bool getBool(juce::String name)
    {
        auto val = settingsMap.at(name).toLowerCase();
        if (val.startsWithChar('f'))
        {
            return false;
        }
        return true;
    }
    void set(juce::String name, juce::String val)
    {
        settingsMap[name] = val;
    }
    void setBool(juce::String name, bool val)
    {
        juce::String boolString = val ? "true" : "false";
        settingsMap[name] = boolString;
    }
    void flipBool(juce::String name)
    {
        bool val = getBool(name);
        val = !val;
        juce::String boolString = val ? "true" : "false";
        settingsMap[name] = boolString;
    }
    //
    std::map<juce::String, juce::String> settingsMap = {
        {"updates", "true"},
        {"motdTag", "x"}
    };
    juce::String projectName;
};

