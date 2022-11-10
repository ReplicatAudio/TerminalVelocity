/*
  ==============================================================================

    RAActionMsg.h
    Created: 7 Nov 2022 8:29:48pm
    Author:  dombr

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
class RAActionMsg
{
public:
    RAActionMsg(juce::String newActionPrefix, juce::String newAction, int newSubId = -1)
    {
        setMessage(newActionPrefix, newAction, newSubId);
    }
    RAActionMsg(juce::String newMessageString)
    {
        setMessageFromString(newMessageString);
    }

    // implicit conversion to string
    operator juce::String() const { return messageString; }

    juce::String getMessageString()
    {
        return messageString;
    }
    void setMessage(juce::String newActionPrefix, juce::String newAction, int newSubId = -1)
    {
        messageString = newActionPrefix + "." + newAction + "." + juce::String(newSubId);
        prefix = newActionPrefix;
        action = newAction;
        subId = newSubId;
    }
    void setMessageFromString(juce::String newMessageString)
    {
        messageString = newMessageString;

        int lastdelimiter = newMessageString.indexOfChar('.');
        prefix = newMessageString.substring(0, lastdelimiter);

        newMessageString = newMessageString.substring(lastdelimiter + 1);// erase part of string
        lastdelimiter = newMessageString.indexOfChar('.');
        action = newMessageString.substring(0, lastdelimiter);

        newMessageString = newMessageString.substring(lastdelimiter + 1);// erase part of string
        subId = newMessageString.getIntValue();
    }
    juce::String getPrefix()
    {
        return prefix;
    }
    void setPrefix(juce::String newPrefix)
    {
        prefix = newPrefix;
        setMessage(newPrefix, action, subId);
    }
    juce::String getAction()
    {
        return action;
    }
    void setAction(juce::String newAction)
    {
        action = newAction;
        setMessage(prefix, newAction, subId);
    }
    int getSubId()
    {
        return subId;
    }
    void setSubId(int newSubId)
    {
        subId = newSubId;
        setMessage(prefix, action, newSubId);
    }
private:
    juce::String messageString;
    juce::String prefix;
    juce::String action;
    int subId;
};