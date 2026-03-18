/*
    MainComponent.cpp

    Part of MR Lab Control Software

    SPDX-License-Identifier: EUPL-1.2
    SPDX-FileCopyrightText: Copyright (c) 2025 Martin Rumori/sonible GmbH
 */

#include "MainComponent.h"
#include "AppControlComponent.h"
#include <controller/MainController.h>
#include <controller/AppHandle.h>

namespace mrlab::view
{

MainComponent::MainComponent (controller::MainController& controller)
    : mainController (controller),
      loggingComponent (controller.getLogger())
{
    addAndMakeVisible (loggingComponent);

    setSize (800, 700);
}

void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    loggingComponent.setBounds (getLocalBounds().withHeight (300).reduced (10));
}

} // namespace mrlab::view
