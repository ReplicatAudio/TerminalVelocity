/*
  ==============================================================================

    Theme.h
    Created: 12 Jun 2022 5:46:59pm
    Author:  Mathieu Dombrock

    v3

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <map>

////////////////
class RAPallet
{
public:
    RAPallet()
    {

    }
    ~RAPallet() {}
    void setAll(
        juce::Array<int> newBg,
        juce::Array<int> newBg2,
        juce::Array<int> newMain,
        juce::Array<int> newLight,
        juce::Array<int> newAccent
    )
    {
        bg = newBg;
        bg2 = newBg2;
        main = newMain;
        light = newLight;
        accent = newAccent;
    }
    void setBg(juce::Array<int> newBg)
    {
        bg = newBg;
    }
    void setBg2(juce::Array<int> newBg2)
    {
        bg2 = newBg2;
    }
    void setMain(juce::Array<int> newMain)
    {
        main = newMain;
    }
    void setLight(juce::Array<int> newLight)
    {
        light = newLight;
    }
    void setAccent(juce::Array<int> newAccent)
    {
        accent = newAccent;
    }
    juce::Colour getBg(float factor = 1.0f)
    {
        juce::Colour out = buildColour(bg, factor);
        return out;
    }
    juce::Colour getBg2(float factor = 1.0f)
    {
        juce::Colour out = buildColour(bg2, factor);
        return out;
    }
    juce::Colour getMain(float factor = 1.0f)
    {
        juce::Colour out = buildColour(main, factor);
        return out;
    }
    juce::Colour getLight(float factor = 1.0f)
    {
        juce::Colour out = buildColour(light, factor);
        return out;
    }
    juce::Colour getAccent(float factor = 1.0f)
    {
        juce::Colour out = buildColour(accent, factor);
        return out;
    }

private:
    juce::Colour buildColour(juce::Array<int> rgb, float factor) {
        float f = 0.75f + (factor * 0.25f);
        int r = rgb[0];
        int g = rgb[1];
        int b = rgb[2];
        r *= f;
        g *= f;
        b *= f;
        r = juce::jmin(r, 255);
        g = juce::jmin(g, 255);
        b = juce::jmin(b, 255);
        return juce::Colour(r, g, b);
    }
    juce::Array<int> bg = { 41, 63, 64 };
    juce::Array<int> bg2 = { 112, 96, 177 };
    juce::Array<int> main = { 106, 153, 87 };
    juce::Array<int> light = { 227, 235, 223 };
    juce::Array<int> accent = { 162, 207, 121 };
};

class RATheme
{
public:
    RATheme(juce::AudioProcessorEditor* editorRef)
    {
        initColours();
        setTheme(editorRef);
    };
    ~RATheme() {};
    std::map<juce::String, RAPallet> pallets;
    juce::String selectedPallet = "standard";
    juce::AudioProcessorEditor* editor;
    void setPallet(juce::String palletName)
    {
        if (pallets.count(palletName))
        {
            selectedPallet = palletName;
            setTheme(editor);
        }
        else {
            jassertfalse;
        }
    }
    juce::Array<juce::String> listPallets()
    {
        juce::Array<juce::String> out;
        for (const auto& pair : pallets) {
            out.add(pair.first);
        }
        return out;
    }
    // Wrappers for pallet easy access
    juce::Colour getBg(float factor = 1.0f)
    {
        juce::Colour out = pallets[selectedPallet].getBg(factor);
        return out;
    }
    juce::Colour getBg2(float factor = 1.0f)
    {
        juce::Colour out = pallets[selectedPallet].getBg2(factor);
        return out;
    }
    juce::Colour getMain(float factor = 1.0f)
    {
        juce::Colour out = pallets[selectedPallet].getMain(factor);
        return out;
    }
    juce::Colour getLight(float factor = 1.0f)
    {
        juce::Colour out = pallets[selectedPallet].getLight(factor);
        return out;
    }
    juce::Colour getAccent(float factor = 1.0f)
    {
        juce::Colour out = pallets[selectedPallet].getAccent(factor);
        return out;
    }
    void initColours()
    {
        RAPallet standard;
        standard.setBg({ 44,44,48 });
        standard.setBg2({ 82, 11, 40 });
        standard.setMain({ 218, 185, 52 });
        standard.setLight({ 248, 233, 128 });
        standard.setAccent({ 100, 250, 212 });
        pallets.insert({ "standard", standard });


        RAPallet iceBlue;
        iceBlue.setBg({ 9, 46, 70 });
        iceBlue.setBg2({ 75, 155, 171 });
        iceBlue.setMain({ 79, 187, 215 });
        iceBlue.setLight({ 175, 245, 229 });
        iceBlue.setAccent({ 175, 84, 42 });
        pallets.insert({ "iceblue", iceBlue });

        RAPallet readit;
        readit.setBg({ 9, 46, 70 });
        readit.setBg2({ 25, 155, 171 });
        readit.setMain({ 39, 187, 215 });
        readit.setLight({ 245, 245, 229 });
        readit.setAccent({ 255, 84, 42 });
        pallets.insert({ "readit", readit });

        RAPallet forest;
        forest.setBg({ 57, 43, 24 });
        forest.setBg2({ 110, 86, 33 });
        forest.setMain({ 146, 103, 33 });
        forest.setLight({ 146, 162, 104 });
        forest.setAccent({ 236, 239, 220 });
        pallets.insert({ "forest", forest });

        RAPallet allBlack;
        allBlack.setBg({ 0,0,0 });
        allBlack.setBg2({ 0,0,0 });
        allBlack.setMain({ 0,0,0 });
        allBlack.setLight({ 0,0,0 });
        allBlack.setAccent({ 0,0,0 });
        pallets.insert({ "allblack", allBlack });
    }

    void setTheme(juce::AudioProcessorEditor* editorRef) {

        editor = editorRef;
        initColours();
        editor->getLookAndFeel().setDefaultSansSerifTypeface(getCustomTypeface());

        editor->getLookAndFeel().setColour(juce::Slider::textBoxTextColourId, getMain(1));
        editor->getLookAndFeel().setColour(juce::Slider::textBoxOutlineColourId, getBg(1));
        editor->getLookAndFeel().setColour(juce::Slider::textBoxHighlightColourId, getBg(1));
        editor->getLookAndFeel().setColour(juce::Slider::textBoxBackgroundColourId, getBg(1));

        editor->getLookAndFeel().setColour(juce::Slider::thumbColourId, getMain(4));
        editor->getLookAndFeel().setColour(juce::Slider::trackColourId, getMain(1));
        editor->getLookAndFeel().setColour(juce::Slider::rotarySliderFillColourId, getMain(1));
        editor->getLookAndFeel().setColour(juce::Slider::rotarySliderOutlineColourId, getBg(2));
        editor->getLookAndFeel().setColour(juce::Slider::backgroundColourId, getBg(2));

        editor->getLookAndFeel().setColour(juce::ComboBox::backgroundColourId, getBg(2));
        editor->getLookAndFeel().setColour(juce::ComboBox::textColourId, getMain(1));
        editor->getLookAndFeel().setColour(juce::ComboBox::arrowColourId, getMain(1));
        editor->getLookAndFeel().setColour(juce::ComboBox::outlineColourId, getBg(1));
        editor->getLookAndFeel().setColour(juce::ComboBox::buttonColourId, getMain(1));

        editor->getLookAndFeel().setColour(juce::PopupMenu::backgroundColourId, getBg(2));
        editor->getLookAndFeel().setColour(juce::PopupMenu::textColourId, getBg2(1));
        editor->getLookAndFeel().setColour(juce::PopupMenu::highlightedBackgroundColourId, getAccent(1));
        editor->getLookAndFeel().setColour(juce::PopupMenu::highlightedTextColourId, getLight(1));

        editor->getLookAndFeel().setColour(juce::TextButton::buttonColourId, getBg(2));
        editor->getLookAndFeel().setColour(juce::TextButton::textColourOffId, getMain(1));

        editor->getLookAndFeel().setColour(juce::ToggleButton::textColourId, getMain(1));
        editor->getLookAndFeel().setColour(juce::ToggleButton::tickColourId, getAccent(1));
        editor->getLookAndFeel().setColour(juce::ToggleButton::tickDisabledColourId, getMain(1));

        editor->getLookAndFeel().setColour(juce::AlertWindow::backgroundColourId, getBg(1));
        editor->getLookAndFeel().setColour(juce::AlertWindow::textColourId, getMain(1));
        editor->getLookAndFeel().setColour(juce::AlertWindow::outlineColourId, getMain(1));

        editor->getLookAndFeel().setColour(juce::DialogWindow::backgroundColourId, getBg(1));
        editor->getLookAndFeel().setColour(juce::DialogWindow::textColourId, getMain(1));

        editor->getLookAndFeel().setColour(juce::DocumentWindow::backgroundColourId, getBg(1));
        editor->getLookAndFeel().setColour(juce::DocumentWindow::textColourId, getBg2(1));
        editor->getLookAndFeel().setColour(juce::DocumentWindow::textColourId, getMain(1));

        editor->getLookAndFeel().setColour(juce::ListBox::backgroundColourId, getAccent(1));
        editor->getLookAndFeel().setColour(juce::ListBox::textColourId, getMain(1));
        editor->getLookAndFeel().setColour(juce::ListBox::outlineColourId, getBg(2));

        editor->getLookAndFeel().setColour(juce::TextEditor::backgroundColourId, getBg(2));
        editor->getLookAndFeel().setColour(juce::TextEditor::focusedOutlineColourId, getBg(2));
        editor->getLookAndFeel().setColour(juce::TextEditor::highlightColourId, getBg2(1));
        editor->getLookAndFeel().setColour(juce::TextEditor::highlightedTextColourId, getAccent(1));
        editor->getLookAndFeel().setColour(juce::TextEditor::outlineColourId, getBg(2));
        editor->getLookAndFeel().setColour(juce::TextEditor::shadowColourId, getBg(2));
        editor->getLookAndFeel().setColour(juce::TextEditor::textColourId, getMain(1));

        editor->getLookAndFeel().setColour(juce::MidiKeyboardComponent::blackNoteColourId, getBg2(1));
        editor->getLookAndFeel().setColour(juce::MidiKeyboardComponent::whiteNoteColourId, getBg(2));
        editor->getLookAndFeel().setColour(juce::MidiKeyboardComponent::keyDownOverlayColourId, getMain(1));
        editor->getLookAndFeel().setColour(juce::MidiKeyboardComponent::keySeparatorLineColourId, getBg(2));
        editor->getLookAndFeel().setColour(juce::MidiKeyboardComponent::mouseOverKeyOverlayColourId, getMain(1));
        editor->getLookAndFeel().setColour(juce::MidiKeyboardComponent::textLabelColourId, getMain(1));
        editor->getLookAndFeel().setColour(juce::MidiKeyboardComponent::upDownButtonArrowColourId, getMain(1));
        editor->getLookAndFeel().setColour(juce::MidiKeyboardComponent::upDownButtonBackgroundColourId, getBg(2));

        editor->getLookAndFeel().setColour(juce::ScrollBar::thumbColourId, getBg(2));
        editor->getLookAndFeel().setColour(juce::ScrollBar::backgroundColourId, getBg(1));
        editor->getLookAndFeel().setColour(juce::ScrollBar::trackColourId, getBg(2));

        editor->getLookAndFeel().setColour(juce::Label::backgroundColourId, getBg(1));
        editor->getLookAndFeel().setColour(juce::Label::outlineColourId, getBg(1));
        editor->getLookAndFeel().setColour(juce::Label::textColourId, getMain(1));
    }
    static const juce::Font getCustomFont()
    {
        static auto typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::EvolventaRegular_ttf, BinaryData::EvolventaRegular_ttfSize);
        return juce::Font(typeface);
    }
    static juce::Typeface::Ptr getCustomTypeface()
    {
        static auto typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::EvolventaRegular_ttf, BinaryData::EvolventaRegular_ttfSize);
        return typeface;
    }
};