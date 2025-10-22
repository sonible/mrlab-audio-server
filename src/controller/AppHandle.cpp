/*
    AppHandle.cpp

    Part of MR Lab Control Software

    SPDX-License-Identifier: EUPL-1.2
    SPDX-FileCopyrightText: Copyright (c) 2025 Martin Rumori/sonible GmbH
 */

#include "AppHandle.h"

namespace mrlab::controller
{

AppHandle::AppHandle (AppConfig newConfig)
    : config (std::move (newConfig))
{}

AppHandle::AppState AppHandle::start()
{
    if (isRunning())
        return state;

    int streamFlags = (config.captureStdOut ? juce::ChildProcess::wantStdOut : 0) |
                      (config.captureStdErr ? juce::ChildProcess::wantStdErr : 0);

    setStateAndNotify (process.start (config.startCommand, streamFlags) ? AppState::alive : AppState::startFailed);
    startTimerHz (stateUpdateHz);

    // TODO: implement waiting for app to become ready (IMRV-33)
    // for now, we just become ready 3 seconds after launching.
    juce::Timer::callAfterDelay (3000, [&] {
        if (state != AppState::alive)
            return;

        setStateAndNotify (AppState::ready);
    });

    return state;
}

AppHandle::AppState AppHandle::stop()
{
    if (! isRunning())
        return state;

    if (config.stopCommand.isEmpty())
        kill();

    // TODO: implement app stop request (IMRV-32)

    setStateAndNotify (AppState::stopRequested);

    return state;
}


AppHandle::AppState AppHandle::kill()
{
    if (! process.isRunning())
        return state;

    setStateAndNotify (process.kill() ? AppState::killed : AppState::killFailed);

    return state;
}

uint32_t AppHandle::getExitCode() const
{
    jassert (isFinished());

    return process.getExitCode();
}

void AppHandle::timerCallback()
{
    updateState();

    if (! isRunning())
        stopTimer();

    updateOutput();
}

void AppHandle::setStateAndNotify (AppState newState)
{
    if (state == newState)
        return;

    state = newState;
    listeners.call (&Listener::appStateChanged, *this, state);
}

void AppHandle::updateState()
{
    const auto processRunning = process.isRunning();

    if (! isRunning() && processRunning) // note: process.isRunning() != this.isRunning()!
    {
        jassertfalse; // zombie?
        return;
    }

    // Previously running app stopped execution?
    if (isRunning() && ! processRunning)
        setStateAndNotify (process.getExitCode() == 0 ? AppState::stoppedSuccess : AppState::stoppedError);

    // TODO: Implement crash detection of app (IMRV-35).
}

void AppHandle::updateOutput()
{
    // Both juce::ChildProcess:readAllProcessOutput() (as stated in
    // the docs) and juce::ChildProcess::readProcessOutput() (as
    // cannot be guessed from the docs) have blocking behaviour, so
    // are unusable in a synchronous context. Functionality disabled
    // for now.
    // TODO: IMRV-39
    // output = process.readAllProcessOutput();

    if (output.isNotEmpty())
        listeners.call (&Listener::appOutputAvailable, *this, output);
}

} // namespace mrlab::controller
