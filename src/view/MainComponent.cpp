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

MainComponent::MainComponent (controller::MainController& controller)
    : mainController (controller),
      webGuiButton ("MainComponent::webGuiButton"),
      reloadButton ("MainComponent::reloadButton")
{
    setSize (600, 400);

    webGuiButton.setButtonText ("Open WebGUI");
    webGuiButton.setClickingTogglesState (true);

    webGuiButton.onClick = [&] {
        webGuiButton.getToggleState() ? launchWebGui() : webGui.reset();
    };

    reloadButton.setButtonText ("Reload");

    reloadButton.onClick = [&] {
        if (webGui)
            webGui->refresh();
    };

    addAndMakeVisible (webGuiButton);
    addChildComponent (reloadButton);
}

void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    webGuiButton.setBounds (20, 20, 100, 30);
    reloadButton.setBounds (150, 20, 100, 30);

    if (webGui)
        webGui->setBounds (getBounds().withTop (70));
}

void MainComponent::launchWebGui()
{
    auto options = juce::WebBrowserComponent::Options()
                   .withBackend (juce::WebBrowserComponent::Options::Backend::webview2)
                   .withWinWebView2Options (juce::WebBrowserComponent::Options::WinWebView2()
                                            .withUserDataFolder (juce::File::getSpecialLocation (juce::File::SpecialLocationType::tempDirectory)));

    webGui = std::make_unique<juce::WebBrowserComponent> (options);
    webGui->goToURL ("http://localhost:8080");
    addAndMakeVisible (*webGui);
    reloadButton.setVisible (true);
    resized();
}

void MainComponent::closeWebGui()
{
    if (webGui == nullptr)
        return;

    reloadButton.setVisible (false);
    removeChildComponent (webGui.get());
    webGui.reset();
}

} // namespace mrlab::view
