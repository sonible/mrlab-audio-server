/*
    MainController.cpp

    Part of MR Lab Control Software

    SPDX-License-Identifier: EUPL-1.2
    SPDX-FileCopyrightText: Copyright (c) 2025 Martin Rumori/sonible GmbH
 */

#include "MainController.h"
#include <CivetServer.h>
#include <juce_core/juce_core.h>
#include <iostream>

namespace mrlab::controller
{

MainController::MainController()
{
    std::vector<std::string> civetOptions;

#ifdef JUCE_WIN
    const auto documentRoot = juce::File::getSpecialLocation (juce::File::userApplicationDataDirectory).getChildFile ("mrlabctrl").getChildFile ("webgui");
    // juce::SystemStats::getEnvironmentVariable ("APPDATA", "");
#else
    const auto documentRoot = juce::File::getSpecialLocation (juce::File::userApplicationDataDirectory).getChildFile ("Application Support").getChildFile ("mrlabctrl").getChildFile ("webgui");
#endif

    // if (! documentRoot.isDirectory())
    //     documentRoot.createDirectory();

    std::cout << "WebGUI document root is: " << documentRoot.getFullPathName() << std::endl;

    civetOptions.push_back ("document_root");
    civetOptions.push_back (documentRoot.getFullPathName().toStdString());
    civetOptions.push_back ("listening_ports");
    civetOptions.push_back ("8080");

    mg_init_library (0);
    civetServer = std::make_unique<CivetServer> (civetOptions);

    civetServer->addWebSocketHandler ("/ws", this);
}

MainController::~MainController()
{
    civetServer.reset();
    mg_exit_library();
}

bool MainController::handleConnection (CivetServer* server, const struct mg_connection* conn)
{
    std::cout << "WebSocket connection request" << std::endl;

    return true;
}

void MainController::handleReadyState (CivetServer* server, struct mg_connection* conn)
{
    std::cout << "WebSocket connection ready" << std::endl;
}

bool MainController::handleData (CivetServer* server, struct mg_connection* conn, int bits, char* data, size_t data_len)
{
    std::cout << "WebSocket data received: " << data << std::endl;

    return true;
}

void MainController::handleClose (CivetServer* server, const struct mg_connection* conn)
{
    std::cout << "WebSocket connection closing" << std::endl;
}

} // namespace mrlab::controller
