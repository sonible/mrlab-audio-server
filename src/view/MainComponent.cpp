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
    mainController.getAppController().addListener (this);

    addAndMakeVisible (loggingComponent);

    setSize (800, 300);
}

MainComponent::~MainComponent()
{
    mainController.getAppController().removeListener (this);
}

void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    loggingComponent.setBounds (getLocalBounds().withHeight (300).reduced (10));

    auto y = loggingComponent.getBottom();
    for (auto& control : appControls)
        control->setBounds (50, y += 50, getWidth() - 100, 35);
}

void MainComponent::appAdded (controller::AppHandle& app)
{
    auto& c = appControls.emplace_back (std::make_unique<AppControlComponent> (app));
    addAndMakeVisible (*c);
    setSize (800, 300 + 50 * int (appControls.size() + 1));
}

void MainComponent::appWillBeRemoved (controller::AppHandle& app)
{
    const auto it = std::find_if (appControls.begin(), appControls.end(), [id = app.getId()] (auto& c) { return c->getId() == id; });

    if (it == appControls.end())
        return;

    removeChildComponent (it->get());
    appControls.erase (it);
    setSize (800, 300 + 50 * int (appControls.size() + 1));
}

} // namespace mrlab::view
