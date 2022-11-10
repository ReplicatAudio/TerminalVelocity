/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ZmidiAudioProcessor::ZmidiAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       //.withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       //.withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
    , fileIO("C:\\ReplicatAudio\\TerminalVelocity")
{
    addParameter(p1 = new juce::AudioParameterInt("p1", "slider 1", -127, 127, 0));
    addParameter(p2 = new juce::AudioParameterInt("p2", "slider 2", -127, 127, 100));
    settings.load(fileIO);
}

ZmidiAudioProcessor::~ZmidiAudioProcessor()
{
}

//==============================================================================
const juce::String ZmidiAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ZmidiAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ZmidiAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ZmidiAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ZmidiAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ZmidiAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ZmidiAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ZmidiAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ZmidiAudioProcessor::getProgramName (int index)
{
    return {};
}

void ZmidiAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ZmidiAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void ZmidiAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ZmidiAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ZmidiAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    buffer.clear();

    juce::MidiBuffer processedMidi;

    for (const auto metadata : midiMessages)
    {
        auto message = metadata.getMessage();
        const auto time = metadata.samplePosition;

        if (message.isNoteOn())
        {
            notesOn++;
            noteInVel = message.getVelocity();
            switch (selectedType) {
                case 1:// Gain
                    noteModVel = p1->get();
                    noteOutVel = noteInVel + noteModVel;
                    if (noteOutVel < 0) {
                        noteOutVel = 0;
                    };
                    break;
                case 2:// Absolute
                    noteModVel = p1->get();
                    noteOutVel = noteModVel;
                    break;
                case 3:// Min/Max
                    min = p1->get();
                    max = p2->get();
                    noteOutVel = noteInVel;
                    if (noteOutVel > max) {
                        noteOutVel = max;
                    }
                    if (noteOutVel < min) {
                        noteOutVel = min;
                    }
                    break;
            }
            message = juce::MidiMessage::noteOn(message.getChannel(),
                message.getNoteNumber(),
                (juce::uint8)noteOutVel);
            cb.sendChangeMessage();
        }
        if (message.isNoteOff()) {
            notesOn--;
            cb.sendChangeMessage();
        }
        processedMidi.addEvent(message, time);
        //
        
        //
    }

    midiMessages.swapWith(processedMidi);
}

//==============================================================================
bool ZmidiAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ZmidiAudioProcessor::createEditor()
{
    return new ZmidiAudioProcessorEditor (*this);
}

//==============================================================================
void ZmidiAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ZmidiAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ZmidiAudioProcessor();
}
