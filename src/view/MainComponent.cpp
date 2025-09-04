/*
    MainComponent.cpp

    Part of MR Lab Control Software

    SPDX-License-Identifier: EUPL-1.2
    SPDX-FileCopyrightText: Copyright (c) 2025 Martin Rumori/sonible GmbH
 */

#include "MainComponent.h"
#include <controller/MainController.h>

namespace mrlab::view
{

MainComponent::MainComponent (controller::MainController& mainController)
    : mainController (mainController)
{
    setSize (600, 400);
}

void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{}

} // namespace mrlab::view
