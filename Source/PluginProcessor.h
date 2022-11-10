/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/
/*
Need To update this to use the JUCE state system
Right now there is a bug where when you change types and go to another plugin you lose your type when coming back
This was fixed with the slider valies but that is because they are handled in the processor and not the editor
There is also a bug where the second slider cant seem to be set to a value over 10
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class ZmidiAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    ZmidiAudioProcessor();
    ~ZmidiAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    int noteModVel = 0;
    int noteInVel = 0;
    int noteOutVel = 0;
    int min = 0;
    int max = 127;
    int selectedType = 3;
    int notesOn = 0;
    juce::AudioParameterInt* p1;
    juce::AudioParameterInt* p2;

    juce::ChangeBroadcaster cb;
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ZmidiAudioProcessor)
};
