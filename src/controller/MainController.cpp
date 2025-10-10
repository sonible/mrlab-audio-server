/*
    MainController.cpp

    Part of MR Lab Control Software

    SPDX-License-Identifier: EUPL-1.2
    SPDX-FileCopyrightText: Copyright (c) 2025 Martin Rumori/sonible GmbH
 */

#include "MainController.h"
#include <juce_events/juce_events.h>

namespace mrlab::controller
{

MainController::MainController()
    : appController (appConfigController)
{
    // Simulate dynamic run-time adding after construction time...
    juce::Timer::callAfterDelay(1200, [&] {
        appController.add (AppConfigController::testConfig0);
        appController.add (AppConfigController::testConfig1);
    });
}

} // namespace mrlab::controller
