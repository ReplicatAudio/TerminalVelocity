/*
  ==============================================================================

    RAHttp.h
    Created: 6 Nov 2022 6:01:17pm
    Author:  dombr

    Example:
    RAHttpResponse test = RAHttp::get("https://replicataudio.com");
    DBG(test.body);
  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class RAHttpResponse
{
public:
    int status=0;
    juce::String headers="";
    juce::String body="";
};

class RAHttp
{
public:
    static RAHttpResponse get(juce::String urlString, int timeoutMs=1000)
    {
        const juce::URL& url = urlString;
        RAHttpResponse res;
        juce::StringPairArray responseHeaders;
        int statusCode = 0;
        if (auto stream = url.createInputStream(juce::URL::InputStreamOptions(juce::URL::ParameterHandling::inAddress)
            .withConnectionTimeoutMs(timeoutMs)
            .withResponseHeaders(&responseHeaders)
            .withStatusCode(&statusCode)))
        {
            res.status = statusCode;
            res.headers = responseHeaders.getDescription();
            res.body = stream->readEntireStreamAsString();
            return res;
        }

        if (statusCode != 0)
        {
            res.status = statusCode;
            return res;
        }
        return res;
    }
};
