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
    LoggingComponent (Logger& loggerInstance);

    ~LoggingComponent() override;

    void messageLogged (const juce::String& message) override;

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    juce::Viewport viewport;
    juce::Component container;

    Logger& logger;

    std::list<std::unique_ptr<juce::Label>> logLines;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoggingComponent)
};

} // namespace mrlab::view
