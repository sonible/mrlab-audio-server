/*
    TotalmixController.cpp

    Part of MR Lab Control Software

    SPDX-License-Identifier: EUPL-1.2
    SPDX-FileCopyrightText: Copyright (c) 2026 Martin Rumori/sonible GmbH
 */

#include "TotalmixController.h"
#include "OscController.h"
#include "SubPathOscAgentWithEndpoint.h"
#include "UdpEndpoint.h"
#include <Globals.h>
#include <util/Logger.h>

namespace mrlab::controller
{

TotalmixController::TotalmixController (OscController& oscControllerIn)
    : oscController (oscControllerIn)
{}

TotalmixController::~TotalmixController()
{}


bool TotalmixController::start()
{
    const auto listeningPort = Globals::getTotalmixListeningPort();
    const auto destination = lo::Address (Globals::getTotalmixControlHost(),
                                          Globals::getTotalmixControlPort());
    const auto subPath = osc::Address::totalmix;

    try
    {
        auto endpoint = std::make_unique<UdpEndpointWithPathRewrite> (oscController,
                                                                      listeningPort,
                                                                      destination,
                                                                      subPath);
        endpoint->start();

        agent = std::make_unique<SubPathOscAgentWithEndpoint> (oscController,
                                                               subPath,
                                                               std::move (endpoint));
    }
    catch (const std::runtime_error& e)
    {
        Logger::logError (juce::String ("TotalmixController: Could not start: ") + e.what());
        return false;
    }

    return true;
}

void TotalmixController::stop()
{
    agent.reset();
}

} // namespace mrlab::controller
