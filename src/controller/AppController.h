/*
    AppController.h

    Part of MR Lab Control Software

    SPDX-License-Identifier: EUPL-1.2
    SPDX-FileCopyrightText: Copyright (c) 2025 Martin Rumori/sonible GmbH
 */

#pragma once

#include <map>
#include <juce_core/juce_core.h>
#include <util/ListenerInterface.h>

namespace mrlab::controller
{

class AppHandle;
class AppConfigController;

//==============================================================================
class AppController
{
public:
    //==============================================================================
    /** Exception that is thrown when there is no app for the given id. */
    class AppUnknownException : std::exception
    {
    public:
        AppUnknownException (const juce::Identifier& appId)
            : msg ("AppUnknownException: no app found with id " + appId.toString())
        {}

        const char* what() const noexcept override { return msg.toUTF8(); }

    protected:
        juce::String msg;
    };

    //==============================================================================
    /** Listener interface to get informed about currently managed apps. */
    struct Listener
    {
        virtual ~Listener() = default;

        /** Indicates that an app was added to the controller.

            @param app App that was added.
         */
        virtual void appAdded (AppHandle& app) = 0;

        /** Indicates that an app is about to be removed from the controller.

            The corresponding app handle will be deleted after this
            call has returned, thus app will become invalid and should
            only be used within this call.

            @param app App that is going to be removed.
         */
        virtual void appWillBeRemoved (AppHandle& app) = 0;
    };

    //==============================================================================
    AppController (AppConfigController& configController);

    ~AppController();

    /** Add an app handle for appId to this controller.

        This will acquire a matching app configuration for instantiating a handle.
        If there is already a handle for appId, it should be removed before calling this.

        @param appId Unique app identifier.
        @returns true on success, false if there is already an app with this id.
        @throws AppConfigNotFoundException.
     */
    bool add (const juce::Identifier& appId);

    /** Remove the app handle for appId from this controller.

        This is only supported for apps in a dead or unknown state.

        @param appId Unique app identifier.
        @returns true on success, false if the app is in a running state.
        @throws AppUnknownException.
     */
    bool remove (const juce::Identifier& appId);

    /** Get the app handle for appId.

        @returns The matching handle for appId.
        @throws AppUnknownException.
     */
    AppHandle& getApp (const juce::Identifier& appId);

    /** Get the const app handle for appId.

        @returns The matching const handle for appId.
        @throws AppUnknownException.
     */
    const AppHandle& getApp (const juce::Identifier& appId) const;

    /** @returns a const reference to the map of managed apps. */
    const std::map<juce::Identifier, std::unique_ptr<AppHandle>>& getApps() { return apps; }

private:
    //==============================================================================
    /** Check whether we know about an appId and throw an exception otherwise. */
    void checkForAppAndThrowIfNotFound (const juce::Identifier& appId) const;

    //==============================================================================
    std::map<juce::Identifier, std::unique_ptr<AppHandle>> apps;

    AppConfigController& appConfigController;

    MRLAB_IMPLEMENT_LISTENER_INTERFACE
};

} // namespace mrlab::controller
