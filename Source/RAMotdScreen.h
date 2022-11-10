/*
  ==============================================================================

    RAMotdScreen.h
    Created: 7 Nov 2022 7:13:28pm
    Author:  dombr

  ==============================================================================
*/

#pragma once
#include "RAHttp.h"
#include "RAStringTools.h"
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "RAActionMsg.h"
class RAMotdScreen : public juce::Component , juce::TextButton::Listener, public juce::ActionBroadcaster
{
public:
    RAMotdScreen() {
        RAHttpResponse motdRes = RAHttp::get(motdURL);
        if (motdRes.status == 200)
        {
            juce::String body = motdRes.body;
            juce::Array<juce::String> bodyArr = RAStringTools::split(body, "|");
            jassert(bodyArr.size() == 4);
            if (bodyArr.size() == 4)
            {
                motdTitle = bodyArr[0];
                motdBody = bodyArr[1];
                motdLink = bodyArr[2];
                motdTag = bodyArr[3];
            }
        }
        else
        {
            jassertfalse;
        }
        addAndMakeVisible(closeBtn);
        closeBtn.setButtonText("ok");
        closeBtn.addListener(this);
        addAndMakeVisible(linkBtn);
        linkBtn.setButtonText("more info");
        linkBtn.addListener(this);
    };
    ~RAMotdScreen() {};
    void buttonClicked(juce::Button* button) override
    {
        if (button == &closeBtn)
        {
            // Close
            RAActionMsg actionMsg("motd", motdTag);
            sendActionMessage(actionMsg);
        }
        if (button == &linkBtn)
        {
            juce::URL(motdLink).launchInDefaultBrowser();
        }
    }
    bool motdOk()
    {
        if (motdTitle == "unset")
        {
            return false;
        }
        return true;
    }
    bool shouldShow(juce::String oldMotdTag)
    {
        if (motdTitle == "unset")
        {
            return false;
        }
        if (motdTag == oldMotdTag)
        {
            return false;
        }
        return true;
    }
    void paint(juce::Graphics& g) override
    {
        g.fillAll(bgColour);
        g.setColour(textColour);
        g.setFont(32);
        g.drawFittedText(motdTitle, 0, 10, getWidth(), 10, juce::Justification::centred, 1);
        g.setFont(16);
        g.drawFittedText(motdBody, 0, 30, getWidth(), 100, juce::Justification::centred, 32);
    }

    void resized() override
    {
        closeBtn.setBounds((getWidth() / 2), getHeight()-38, 100, 32);
        linkBtn.setBounds((getWidth() / 2) - 100, getHeight() - 38, 100, 32);
    }
    void setColours(juce::Colour text, juce::Colour bg)
    {
        textColour = text;
        bgColour = bg;
    }
private:
    juce::Colour textColour = juce::Colours::white;
    juce::Colour bgColour = juce::Colours::darkslategrey;
    juce::TextButton closeBtn;
    juce::TextButton linkBtn;
    juce::String motdTitle = "unset";
    juce::String motdBody = "unset";
    juce::String motdLink = "https://replicataudio.com";
    juce::String motdTag = "unset";
    const juce::String motdURL = "https://replicataudio.com:1234/motd/generic";
};
