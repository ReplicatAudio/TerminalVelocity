/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"



//==============================================================================
ZmidiAudioProcessorEditor::ZmidiAudioProcessorEditor (ZmidiAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), theme(this)
{
    setSize (420, 160);
    // Look and Feel

    // Type Selection
    addAndMakeVisible(typeSelect);
    typeSelect.addItem("Velocity Gain", 1);
    typeSelect.addItem("Absolute Velocity", 2);
    typeSelect.addItem("Min/Max Velocity", 3);
    typeSelect.onChange = [this] { typeSelectChanged(); };
    typeSelect.setSelectedId(audioProcessor.selectedType);
    typeSelect.setSize(420, 30);
    // Sliders
    slider1.setRange(-127, 127, 1);
    slider2.setRange(1, 127, 1);
    slider1.setValue(audioProcessor.p1->get());
    slider2.setValue(audioProcessor.p2->get());
    // This function adds the sliders to the editor
    addAndMakeVisible(&slider1);
    addAndMakeVisible(&slider2);
    slider2.setVisible(false);
    slider1.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 30);
    slider2.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 30);
    slider1.addListener(this);
    slider2.addListener(this);
    // Add the listener to the sliders
    audioProcessor.p1->addListener(this);
    audioProcessor.p2->addListener(this); 
    audioProcessor.cb.addChangeListener(this);

    addAndMakeVisible(raLogoBtn);
    raLogoBtn.addListener(this);
    juce::Image raIconImage = juce::ImageCache::getFromMemory(BinaryData::ra_logo_png, BinaryData::ra_logo_pngSize).rescaled(logoW,logoH, juce::Graphics::highResamplingQuality);
    raLogoBtn.setImages(true, true, true, raIconImage, 0.005f, theme.getMain(0.75f), raIconImage, 0.5f, theme.getMain(1.5f), raIconImage, 0.25f, theme.getMain(2));

    applyThemeTweaks();
}

ZmidiAudioProcessorEditor::~ZmidiAudioProcessorEditor()
{
    audioProcessor.cb.removeChangeListener(this);
    audioProcessor.p1->removeListener(this);
    audioProcessor.p2->removeListener(this);
}

//==============================================================================
void ZmidiAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(theme.getBg(0.5f));

    if (audioProcessor.notesOn>0) {
        g.setColour(theme.getMain(1));
    }
    else {
        g.setColour(theme.getBg2(1));
    }
    juce::Path path;
    float w = getWidth();
    float h = getHeight();
    path.startNewSubPath(juce::Point<float>(w-75.0f, h-20.0f));
    path.lineTo(juce::Point<float>(w-85.0f,h-30.0f));
    path.lineTo(juce::Point<float>(w-75.0f, h-40.0f));
    path.lineTo(juce::Point<float>(w-65.0f, h-30.0f));
    path.closeSubPath();
    g.fillPath(path);

    g.setColour(theme.getMain(1));
    g.fillRect(getWidth()-20, getHeight() - 42, getWidth() / 2, 3);
    g.setColour(theme.getAccent(2));
    g.fillRect(getWidth()-20, getHeight() - 22, getWidth() / 2, 3);

    g.setFont(16.0f);

    g.setColour(theme.getMain(1));
    juce::String inVel = std::to_string(audioProcessor.noteInVel);
    g.drawFittedText(inVel + " <", -20, getHeight() - 50, getWidth(), 20, juce::Justification::right, 1);
    
    g.setColour(theme.getAccent(1));
    juce::String outVel = std::to_string(audioProcessor.noteOutVel);
    g.drawFittedText(outVel + " >", -20, getHeight() - 30, getWidth(), 20, juce::Justification::right, 1);
    
    g.setOpacity(0.5f);//
    juce::String version = ProjectInfo::versionString;
    g.drawFittedText("v"+version, getWidth()-36, 30, getWidth(), 20, juce::Justification::left, 1);
}

void ZmidiAudioProcessorEditor::resized()
{
    // sets the position and size of the slider with arguments (x, y, width, height)
    slider1.setBounds(40, 50, 340, 60);
    slider2.setBounds(210, 50, 170, 60);
    typeSelect.setBounds(0,0,getWidth(), 30);

    raLogoBtn.setBounds(10, getHeight()-logoH, logoW, logoH);
}

void ZmidiAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    audioProcessor.p1->operator= (slider1.getValue());
    audioProcessor.p2->operator= (slider2.getValue());
    repaint();
}

void ZmidiAudioProcessorEditor::typeSelectChanged()
{
    audioProcessor.selectedType = typeSelect.getSelectedId();
    switch (typeSelect.getSelectedId())
    {
        case 1: 
            showOneSlider();
            slider1.setRange(-127.0, 127.0, 1.0);
            break;
        case 2: 
            showOneSlider();
            floorVelocity();
            slider1.setRange(0.0, 127.0, 1.0);
            break;
        case 3:
            showTwoSliders();
            floorVelocity();
            slider1.setRange(0.0, 127.0, 1.0);
            break;
        default: break;
    }
    repaint();
}
// Apply RATheme default overrides
void ZmidiAudioProcessorEditor::applyThemeTweaks()
{
    getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colour(0, 0, 0).withAlpha(0.0f));
    getLookAndFeel().setColour(juce::Slider::rotarySliderOutlineColourId, theme.getBg(0.25f));

    getLookAndFeel().setColour(juce::PopupMenu::backgroundColourId, theme.getBg(2));
    getLookAndFeel().setColour(juce::PopupMenu::textColourId, theme.getMain(1));
    getLookAndFeel().setColour(juce::PopupMenu::highlightedBackgroundColourId, theme.getMain(1));
    getLookAndFeel().setColour(juce::PopupMenu::highlightedTextColourId, theme.getBg(1));
}

void ZmidiAudioProcessorEditor::showOneSlider()
{
    slider1.setBounds(40, 50, 340, 60);
    slider2.setVisible(false);
}
void ZmidiAudioProcessorEditor::showTwoSliders()
{
    slider1.setBounds(40, 50, 170, 60);
    slider2.setVisible(true);
}

void ZmidiAudioProcessorEditor::parameterValueChanged(int 	parameterIndex,
    float 	newValue
)
{
    slider1.setValue(audioProcessor.p1->get());
    slider2.setValue(audioProcessor.p2->get());
    if (typeSelect.getSelectedId() == 3) {//min/max
        // Ensure that max is always at least min
        if (audioProcessor.p1->get() > audioProcessor.p2->get()) {
            audioProcessor.p2->operator= (audioProcessor.p1->get());
        }
    }
}
void ZmidiAudioProcessorEditor::parameterGestureChanged(int 	parameterIndex,
    bool 	gestureIsStarting
) {
    
}

void ZmidiAudioProcessorEditor::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    repaint();
}
void ZmidiAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    if (button == &raLogoBtn)
    {
        // Open URL
    }
}
void ZmidiAudioProcessorEditor::floorVelocity()
{
    if (audioProcessor.p1->get() < 0) {
        audioProcessor.p1->operator= (0);
    }
    if (audioProcessor.p2->get() < 0) {
        audioProcessor.p2->operator= (0);
    }
}
