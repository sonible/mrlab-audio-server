/*
    MainController.h

    Part of MR Lab Control Software

    SPDX-License-Identifier: EUPL-1.2
    SPDX-FileCopyrightText: Copyright (c) 2025 Martin Rumori/sonible GmbH
 */

#pragma once

#include <memory>
#include <CivetServer.h>

namespace mrlab::controller
{
//class CivetServer;

//==============================================================================
class MainController : public CivetWebSocketHandler
{
public:
    //==============================================================================
    MainController();
    ~MainController() override;

    bool handleConnection (CivetServer* server, const struct mg_connection* conn) override;
    void handleReadyState (CivetServer* server, struct mg_connection* conn) override;
    bool handleData (CivetServer* server, struct mg_connection* conn, int bits, char* data, size_t data_len) override;
    void handleClose (CivetServer* server, const struct mg_connection* conn) override;

private:
    //==============================================================================
    std::unique_ptr<CivetServer> civetServer;
};

} // namespace mrlab::controller
