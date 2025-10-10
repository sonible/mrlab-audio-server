/*
    AppController.cpp

    Part of MR Lab Control Software

    SPDX-License-Identifier: EUPL-1.2
    SPDX-FileCopyrightText: Copyright (c) 2025 Martin Rumori/sonible GmbH
 */

#include "AppController.h"
#include "AppHandle.h"
#include "AppConfigController.h"

namespace mrlab::controller
{

AppController::AppController (AppConfigController& configController)
    : appConfigController (configController)
{}

AppController::~AppController()
{}

bool AppController::add (const juce::Identifier& appId)
{
    auto appConfig = appConfigController.findConfig (appId); // may throw

    auto [iter, success] = apps.try_emplace (appId, std::make_unique<AppHandle> (std::move (appConfig)));

    if (success)
        listeners.call (&Listener::appAdded, *iter->second);

    return success;
}

bool AppController::remove (const juce::Identifier& appId)
{
    checkForAppAndThrowIfNotFound (appId);

    auto& app = apps.at (appId);

    if (app->isRunning())
        return false;

    listeners.call (&Listener::appWillBeRemoved, *app);

    apps.erase (appId);

    return true;
}

AppHandle& AppController::getApp (const juce::Identifier& appId)
{
    checkForAppAndThrowIfNotFound (appId);

    return *apps.at (appId);
}

const AppHandle& AppController::getApp (const juce::Identifier& appId) const
{
    checkForAppAndThrowIfNotFound (appId);

    return *apps.at (appId);
}

void AppController::checkForAppAndThrowIfNotFound (const juce::Identifier& appId) const
{
    if (! apps.contains (appId))
        throw AppUnknownException (appId);
}

} // namespace mrlab::controller
