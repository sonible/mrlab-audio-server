/*
    LoggingComponent.cpp

    Part of MR Lab Control Software

    SPDX-License-Identifier: EUPL-1.2
    SPDX-FileCopyrightText: Copyright (c) 2026 Frederik Siepe/sonible GmbH
 */

#include "LoggingComponent.h"

namespace mrlab::view
{

LoggingComponent::LoggingComponent (Logger& loggerInstance)
    : logger (loggerInstance)
{
    addAndMakeVisible (viewport);
    viewport.setViewedComponent (&container);
    viewport.setScrollBarsShown (true, true);

    logger.addListener (this);
}

LoggingComponent::~LoggingComponent()
{
    logger.removeListener (this);
}

void LoggingComponent::messageLogged (const juce::String& message)
{
    const auto lineHeight = 22;

    // create new label
    logLines.push_back (std::make_unique<juce::Label>());
    logLines.back()->setText (message, juce::dontSendNotification);
    logLines.back()->setInterceptsMouseClicks (false, false);
    container.addAndMakeVisible (*logLines.back());

    // set needed size
    const auto font = logLines.back()->getFont();
    const auto neededWidth = int (juce::GlyphArrangement::getStringWidth (font, message));
    logLines.back()->setBounds (0, (int (logLines.size()) - 1) * lineHeight, neededWidth, lineHeight);

    // set colouring if needed
    if (message.startsWith ("Error: ") || message.startsWith ("Fatal: "))
        logLines.back()->setColour (juce::Label::textColourId, juce::Colours::red);
    else if (message.startsWith ("Warn: "))
        logLines.back()->setColour (juce::Label::textColourId, juce::Colours::yellow);

    auto containerWidth = std::max (container.getWidth(), neededWidth);

    // line count exceeded?
    static const auto maxNumLines = 1000;
    if (logLines.size() > maxNumLines)
    {
        // remove one and reposition the other ones
        logLines.pop_front();
        auto posY = 0;
        containerWidth = 0;
        for (auto& line : logLines)
        {
            line->setTopLeftPosition (0, posY);
            containerWidth = std::max (containerWidth, line->getWidth());
            posY += lineHeight;
        }
    }

    // update parent container
    container.setBounds (0, 0, containerWidth, int (logLines.size()) * lineHeight);
    viewport.setViewPosition (0, container.getHeight() - viewport.getViewHeight());
}

void LoggingComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black.withAlpha (0.7f));
}

void LoggingComponent::resized()
{
    viewport.setBounds (getLocalBounds());
}

} // namespace mrlab::view