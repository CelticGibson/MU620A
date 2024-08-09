/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayPluginAudioProcessorEditor::DelayPluginAudioProcessorEditor(DelayPluginAudioProcessor& p,
	juce::AudioProcessorValueTreeState& apvts)
	: AudioProcessorEditor(&p), Audioprocessor(p)
{
	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.
	MasterSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
	MasterSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
	MasterSlider.setTextBoxIsEditable(true);
	
	addAndMakeVisible(MasterSlider);

	MasterLabel.setText("Master", juce::dontSendNotification);
	MasterLabel.attachToComponent(&MasterSlider, false);
	MasterLabel.setJustificationType(juce::Justification::centred);

	addAndMakeVisible(MasterLabel);

	masterAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(apvts,
		"MASTER", MasterSlider));

	MixSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
	MixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
	MixSlider.setTextBoxIsEditable(true);

	addAndMakeVisible(MixSlider);

	MixLabel.setText("Mix", juce::dontSendNotification);
	MixLabel.attachToComponent(&MixSlider, false);
	MixLabel.setJustificationType(juce::Justification::centred);
	addAndMakeVisible(MixLabel);

	mixAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(apvts,
		"MIX", MixSlider));

	FeedbackSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
	FeedbackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
	FeedbackSlider.setTextBoxIsEditable(true);
	

	addAndMakeVisible(FeedbackSlider);

	FeedbackLabel.setText("Feedback", juce::dontSendNotification);
	FeedbackLabel.attachToComponent(&FeedbackSlider, false);
	FeedbackLabel.setJustificationType(juce::Justification::centred);
	addAndMakeVisible(FeedbackLabel);

	feedbackAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(apvts,
		"FEEDBACK", FeedbackSlider));

	DelayTimeSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
	DelayTimeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
	DelayTimeSlider.setTextBoxIsEditable(true);

	addAndMakeVisible(DelayTimeSlider);

	DelayTimeLabel.setText("Delay Time", juce::dontSendNotification);
	DelayTimeLabel.attachToComponent(&DelayTimeSlider, false);
	DelayTimeLabel.setJustificationType(juce::Justification::centred);
	addAndMakeVisible(DelayTimeLabel);

	delayTimeAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(apvts,
		"DELAYTIMEMS", DelayTimeSlider));

	PingPongButton.setClickingTogglesState(true);
	PingPongButton.setButtonText("Ping Pong");
	PingPongButton.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::white);
	PingPongButton.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::white);
	PingPongButton.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::darkred);
	addAndMakeVisible(PingPongButton);

	pingPongAttachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(apvts,
		"PINGPONGENABLED", PingPongButton));

	SyncSelectorButton.setClickingTogglesState(true);
	SyncSelectorButton.setButtonText("Sync Delay");
	SyncSelectorButton.setColour(TextButton::ColourIds::buttonOnColourId, Colours::darkred);
	SyncSelectorButton.setColour(TextButton::ColourIds::textColourOffId, Colours::white);
	SyncSelectorButton.setColour(TextButton::ColourIds::textColourOnId, Colours::white);
	addAndMakeVisible(SyncSelectorButton);

	syncAttachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(apvts,
		"SYNCENABLED", SyncSelectorButton));


	SyncChoice.addItem("Whole Note", 1);
	SyncChoice.addItem("Whole Note Dotted", 2);
	SyncChoice.addItem("Whole Note Triplet", 3);
	SyncChoice.addItem("Half Note", 4);
	SyncChoice.addItem("Half Note Dotted", 5);
	SyncChoice.addItem("Half Note Triplet", 6);
	SyncChoice.addItem("Quarter Note", 7);
	SyncChoice.addItem("Quarter Note Dotted", 8);
	SyncChoice.addItem("Quarter Note Triplet", 9);
	SyncChoice.addItem("Eight Note", 10);
	SyncChoice.addItem("Eight Note Dotted", 11);
	SyncChoice.addItem("Eight Note Triplet", 12);
	SyncChoice.addItem("Sixteenth Note", 13);
	SyncChoice.addItem("Sixteen Note Dotted", 14);
	SyncChoice.addItem("Sixteen Note Triplet", 15);
	SyncChoice.addItem("Thirty Second Note", 16);
	SyncChoice.setJustificationType(juce::Justification::centred);
	addAndMakeVisible(SyncChoice);

	syncChoiceAttachment.reset(new AudioProcessorValueTreeState::ComboBoxAttachment(apvts,
		"SYNCSETTING", SyncChoice));

	ChorusDepthSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
	ChorusDepthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
	ChorusDepthSlider.setTextBoxIsEditable(true);
	addAndMakeVisible(ChorusDepthSlider);

	ChorusDepthLabel.setText("Chorus Depth", juce::dontSendNotification);
	ChorusDepthLabel.attachToComponent(&ChorusDepthSlider, false);
	ChorusDepthLabel.setJustificationType(juce::Justification::centred);
	addAndMakeVisible(ChorusDepthLabel);

	chorusDepthAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(apvts,
		"CHORUSDEPTH", ChorusDepthSlider));

	ChorusRateSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
	ChorusRateSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
	ChorusRateSlider.setTextBoxIsEditable(true);
	addAndMakeVisible(ChorusRateSlider);

	ChorusRateLabel.setText("Chorus Rate", juce::dontSendNotification);
	ChorusRateLabel.attachToComponent(&ChorusRateSlider, false);
	ChorusRateLabel.setJustificationType(juce::Justification::centred);
	addAndMakeVisible(ChorusRateLabel);

	chorusRateAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(apvts,
		"CHORUSRATE", ChorusRateSlider));

	ChorusMixSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
	ChorusMixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
	ChorusMixSlider.setTextBoxIsEditable(true);
	addAndMakeVisible(ChorusMixSlider);

	ChorusMixLabel.setText("Chorus Mix", juce::dontSendNotification);
	ChorusMixLabel.attachToComponent(&ChorusMixSlider, false);
	ChorusMixLabel.setJustificationType(juce::Justification::centred);
	addAndMakeVisible(ChorusMixLabel);

	chorusMixAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(apvts,
		"CHORUSMIX", ChorusMixSlider));

	HighPassButton.setClickingTogglesState(true);
	HighPassButton.setButtonText("High Pass");
	HighPassButton.setColour(TextButton::ColourIds::buttonOnColourId, Colours::darkred);
	HighPassButton.setColour(TextButton::ColourIds::textColourOnId, Colours::white);
	HighPassButton.setColour(TextButton::ColourIds::textColourOffId, Colours::white);
	addAndMakeVisible(HighPassButton);

	highPassAttachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(apvts,
		"HIGHPASSSTATE", HighPassButton));

	CutOffSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
	CutOffSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
	CutOffSlider.setTextBoxIsEditable(true);
	addAndMakeVisible(CutOffSlider);

	CutOffLabel.setText("Cut Off", juce::dontSendNotification);
	CutOffLabel.attachToComponent(&CutOffSlider, false);
	CutOffLabel.setJustificationType(juce::Justification::centred);
	addAndMakeVisible(CutOffLabel);

	cutOffAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(apvts,
		"CUTOFFFREQ", CutOffSlider));

	setSize(850, 600);
}

DelayPluginAudioProcessorEditor::~DelayPluginAudioProcessorEditor()
{
}

//==============================================================================
void DelayPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void DelayPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

	auto leftMargin = getWidth() * 0.02f;
	auto topMargin = getHeight() * 0.05f;
	auto rightMargin = getWidth() * 0.05f;
	auto dialSize = getWidth() * 0.25f;
	auto buttonWidth = dialSize * 0.5f;
	auto buttonHeight = buttonWidth * 0.3f;

	MasterSlider.setBounds(leftMargin, topMargin, dialSize, dialSize);
	
	MixSlider.setBounds(leftMargin + 200, topMargin, dialSize, dialSize);

	FeedbackSlider.setBounds(leftMargin + 400, topMargin, dialSize, dialSize);

	DelayTimeSlider.setBounds(leftMargin + 600, topMargin, dialSize, dialSize);
	
	ChorusDepthSlider.setBounds(leftMargin + 50, topMargin + 250, dialSize/1.6, dialSize/1.6);

	ChorusRateSlider.setBounds(leftMargin + 200, topMargin + 250, dialSize/1.6, dialSize/1.6);

	ChorusMixSlider.setBounds(leftMargin + 50, topMargin + 430, dialSize/1.6, dialSize/1.6);
	
	CutOffSlider.setBounds(leftMargin + 200, topMargin + 430, dialSize/1.6, dialSize/1.6);

	// Position buttons and combo box
	PingPongButton.setBounds(leftMargin + 400, topMargin + 300, buttonWidth, buttonHeight);
	SyncSelectorButton.setBounds(leftMargin + 550, topMargin + 300, buttonWidth, buttonHeight);
	HighPassButton.setBounds(leftMargin + 550, topMargin + 350, buttonWidth, buttonHeight);
	SyncChoice.setBounds(leftMargin + 550, topMargin + 500, buttonWidth + 70, buttonHeight);


}
