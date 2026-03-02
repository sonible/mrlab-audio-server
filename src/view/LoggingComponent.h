/*
    LoggingComponent.h

    Part of MR Lab Control Software

    SPDX-License-Identifier: EUPL-1.2
    SPDX-FileCopyrightText: Copyright (c) 2026 Frederik Siepe/sonible GmbH
 */

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <util/Logger.h>

namespace mrlab::view
{
//==============================================================================
class LoggingComponent : public juce::Component,
                         public Logger::Listener
{
public:
    //==============================================================================
    LoggingComponent (Logger& logger) : logger (logger)
    {
        addAndMakeVisible (viewport);
        viewport.setViewedComponent (&container);
        viewport.setScrollBarsShown (true, false);

        logger.addListener (this);
    }

    ~LoggingComponent() override
    {
        logger.removeListener (this);
    }

    void messageLogged (const juce::String& message)
    {
        const auto lineHeight = 22;

        logLines.push_back (std::make_unique<juce::Label>());
        logLines.back()->setText (message, juce::dontSendNotification);
        container.addAndMakeVisible (*logLines.back());
        logLines.back()->setBounds (0, (logLines.size() - 1) * lineHeight, getWidth(), lineHeight);
        logLines.back()->setInterceptsMouseClicks (false, false);

        if (message.startsWith ("Error: ") || message.startsWith ("Fatal: "))
            logLines.back()->setColour (juce::Label::textColourId, juce::Colours::red);
        else if (message.startsWith ("Warn: "))
            logLines.back()->setColour (juce::Label::textColourId, juce::Colours::yellow);

        container.setBounds (0, 0, getWidth(), logLines.size() * lineHeight);
        viewport.setViewPosition(0, viewport.getViewHeight());
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll (juce::Colours::black.withAlpha (0.7f));
    }

    void resized() override
    {
        viewport.setBounds (getLocalBounds());
    }

private:
    juce::Viewport viewport;
    juce::Component container;

    Logger& logger;

    std::vector<std::unique_ptr<juce::Label>> logLines;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoggingComponent)
};

} // namespace mrlab::view
