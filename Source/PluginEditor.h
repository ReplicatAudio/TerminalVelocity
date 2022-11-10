/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "RATheme.h"

//==============================================================================
/**
*/
class ZmidiAudioProcessorEditor : public juce::AudioProcessorEditor, 
    private juce::Slider::Listener,
    private juce::AudioProcessorParameter::Listener,
    private juce::ChangeListener,
    private juce::Button::Listener
{
public:
    ZmidiAudioProcessorEditor(ZmidiAudioProcessor&);
    ~ZmidiAudioProcessorEditor();

    //===================================================================
    // This is just a standard Juce paint method...
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void sliderValueChanged(juce::Slider* slider) override; // [3]
    void parameterValueChanged(int 	parameterIndex,
        float 	newValue
    ) override;
    void parameterGestureChanged(int 	parameterIndex,
        bool 	gestureIsStarting
    ) override;
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    void buttonClicked(juce::Button* button) override;
    //void paramChanged();
    void typeSelectChanged(); // [3]
    void applyThemeTweaks();
    void showOneSlider();
    void showTwoSliders();
    void floorVelocity();

    //juce::String title = "MIDI Gain";
    juce::ImageButton raLogoBtn;
    //==================================================================
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ZmidiAudioProcessor& audioProcessor;

    juce::Slider slider1; // [1]
    juce::Slider slider2; // [1]

    juce::ComboBox typeSelect;
    // Theme
    RATheme theme;

    const float logoScale = 0.35f;
    const int logoW = 473 * logoScale;
    const int logoH = 80 * logoScale;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ZmidiAudioProcessorEditor)
};
