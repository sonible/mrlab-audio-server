/*
    MainComponent.h

    Part of MR Lab Control Software

    SPDX-License-Identifier: EUPL-1.2
    SPDX-FileCopyrightText: Copyright (c) 2025 Martin Rumori/sonible GmbH
 */

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>

namespace mrlab::controller
{
class MainController;
}

namespace mrlab::view
{

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
    /** Helper to configure and open the webview based user interface. */
    void launchWebGui();

    /** Helper to close the webview based user interface. */
    void closeWebGui();

    //==============================================================================
    controller::MainController& mainController;

    juce::TextButton webGuiButton;
    juce::TextButton reloadButton;
    std::unique_ptr<juce::WebBrowserComponent> webGui;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

} // namespace mrlab::view
