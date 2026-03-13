/*
    TotalmixController.h

    Part of MR Lab Control Software

    SPDX-License-Identifier: EUPL-1.2
    SPDX-FileCopyrightText: Copyright (c) 2026 Martin Rumori/sonible GmbH
 */

#pragma once

#include <juce_core/juce_core.h>

namespace mrlab::controller
{
class OscController;
class SubPathOscAgentWithEndpoint;

//==============================================================================
/** Controller for RME Totalmix remote control via OSC.

    This is merely a SubPathOscAgentWithEndpoint providing a
    transparent OSC communication channel under address "/totalmix".

    All messages sent to this sub-path will be forwarded to the
    configured Totalmix host/port. All messages received from Totalmix
    will be broadcasted to all endpoints (but not looped back to Totalmix).
 */
class TotalmixController
{
public:
    //==============================================================================
    /** Create a TotalmixController.

        @param oscController Reference to the OscController.

        @note Listening/sending to Totalmix will only work after
              start() has been called.
     */
    TotalmixController (OscController& oscControllerIn);
    ~TotalmixController();

    /** Start listening and sub-path message forwarding.

        @returns true on success, false on endpoint failure.
     */
    bool start();

    /** Stop listening and sub-path message forwarding. */
    void stop();

private:
    //==============================================================================
    OscController& oscController;
    std::unique_ptr<SubPathOscAgentWithEndpoint> agent; // Delegate agent.

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TotalmixController)
};

} // namespace mrlab::controller
