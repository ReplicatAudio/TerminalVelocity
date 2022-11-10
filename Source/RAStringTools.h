/*
  ==============================================================================

    RAStringTools.h
    Created: 7 Nov 2022 6:37:26pm
    Author:  dombr

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
class RAStringTools
{
public:
    static juce::Array<juce::String> split(juce::String string, juce::String delimiter)
    {
        jassert(string.isNotEmpty());
        juce::Array<juce::String> out;
        while (string.contains(delimiter))
        {
            out.add(string.substring(0, string.indexOf(delimiter)));
            string = string.substring(string.indexOf(delimiter) + 1);
        }
        // add remaining
        out.add(string);
        return out;
    }
    static juce::String capitalize(juce::String& baseString)
    {
        juce::String firstChar = baseString.substring(0, 1);
        baseString = baseString.replaceSection(0, 1, firstChar.toUpperCase());
        return baseString;
    }
    static juce::String truncDecimals(float input, int decimals = 2)
    {
        juce::String inputS = (juce::String)input;
        juce::Array<juce::String> splitInput = split(inputS, ".");
        juce::String beforeDecimal = splitInput[0];
        juce::String afterDecimal = splitInput[1];
        afterDecimal = afterDecimal.substring(0, decimals);
        if (afterDecimal.isEmpty() == true) {
            afterDecimal = "00";
        }
        juce::String fullString = beforeDecimal + "." + afterDecimal;
        return fullString;
    }
};