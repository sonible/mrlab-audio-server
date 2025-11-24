/*
    OSCMessageOrBundle.h

    Part of MR Lab Control Software

    SPDX-License-Identifier: EUPL-1.2
    SPDX-FileCopyrightText: Copyright (c) 2025 Martin Rumori/sonible GmbH
 */

#pragma once

#include <juce_osc/juce_osc.h>

namespace mrlab::osc
{

//==============================================================================
/** Variant helper class to either represent an OSCMessage or an OSCBundle.

    This serves a similiar purpose as juce::OSCBundle::Element but adds move
    semantics (which the juce implementation lacks).
 */
class OSCMessageOrBundle
{
public:
    //==============================================================================
    /** Construct an OSCMessageOrBundle from an OSC message. */
    template <std::convertible_to<juce::OSCMessage> OSCEntity>
    OSCMessageOrBundle (OSCEntity&& entity)
        : message (std::forward<OSCEntity> (entity))
    {}

    /** Construct an OSCMessageOrBundle from an OSC bundle. */
    template <std::convertible_to<juce::OSCBundle> OSCEntity>
    OSCMessageOrBundle (OSCEntity&& entity)
        : bundle (std::forward<OSCEntity> (entity))
    {}

    /** @returns whether this holds an OSCMessage. */
    bool isMessage() const { return message.has_value(); }

    /** @returns whether this holds an OSCBundle. */
    bool isBundle() const { return bundle.has_value(); }

    /** Get a const reference to the contained OSCMessage.

        @return const reference to the contained OSCMessage
        @throws juce::OSCInternalError
     */
    const juce::OSCMessage& getMessage() const
    {
        ensureMessageOrThrow();
        return *message;
    }

    /** Get a reference to the contained OSCMessage.

        @return reference to the contained OSCMessage
        @throws juce::OSCInternalError
     */
    juce::OSCMessage& getMessage()
    {
        ensureMessageOrThrow();
        return *message;
    }

    /** Get a const reference to the contained OSCBundle.

        @return const reference to the contained OSCBunde
        @throws juce::OSCInternalError
     */
    const juce::OSCBundle& getBundle() const
    {
        ensureBundleOrThrow();
        return *bundle;
    }

    /** Get a reference to the contained OSCBundle.

        @return reference to the contained OSCBunde
        @throws juce::OSCInternalError
     */
    juce::OSCBundle& getBundle()
    {
        ensureBundleOrThrow();
        return *bundle;
    }

    /** Moves the contained OSCMessage out of this.

        @return rvalue reference to the cotained OSCMessage
        @throws juce::OSCInternalError
     */
    operator juce::OSCMessage&&() &&
    {
        ensureMessageOrThrow();
        return *std::move (message);
    }

    /** Moves the contained OSCBundle out of this.

        @return rvalue reference to the cotained OSCBundle
        @throws juce::OSCInternalError
     */
    operator juce::OSCBundle&&() &&
    {
        ensureBundleOrThrow();
        return *std::move (bundle);
    }

private:
    //==============================================================================
    /** Make sure this contains a message, otherwise throw. */
    void ensureMessageOrThrow() const
    {
        if (! message.has_value())
        {
            jassertfalse;
            throw juce::OSCInternalError ("OSCMessageOrBundle: Attempt to access as a message while holding a bundle.");
        }
    }

    /** Make sure this contains a bundle, otherwise throw. */
    void ensureBundleOrThrow() const
    {
        if (! bundle.has_value())
        {
            jassertfalse;
            throw juce::OSCInternalError ("OSCMessageOrBundle: Attempt to access as a bundle while holding a message.");
        }
    }

    //==============================================================================
    std::optional<juce::OSCMessage> message; ///< Message object if holding a message.
    std::optional<juce::OSCBundle> bundle;   ///< Bundle object if holding a bundle.
};

} // namespace mrlab::osc
