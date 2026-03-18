/*
    MainComponent.h

    Part of MR Lab Control Software

    SPDX-License-Identifier: EUPL-1.2
    SPDX-FileCopyrightText: Copyright (c) 2025 Martin Rumori/sonible GmbH
 */

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <controller/AppController.h>
#include "LoggingComponent.h"

namespace mrlab::controller
{
class MainController;
}

namespace mrlab::view
{
class AppControlComponent;

//==============================================================================
class MainComponent : public juce::Component
{
public:
    //==============================================================================
    MainComponent (controller::MainController& controller);

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    //==============================================================================
    controller::MainController& mainController;

    LoggingComponent loggingComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

} // namespace mrlab::view
