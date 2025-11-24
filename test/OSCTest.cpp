/*
    OSCTest.cpp

    Part of MR Lab Control Software

    SPDX-License-Identifier: EUPL-1.2
    SPDX-FileCopyrightText: Copyright (c) 2025 Martin Rumori/sonible GmbH
 */

#include <catch2/catch_test_macros.hpp>
#include <util/osc/OSCHelpers.h>
#include <util/osc/OSCMessageOrBundle.h>

namespace mrlab
{

TEST_CASE ("OSCMessageOrBundle variant helper", "[OSCTest]")
{
    juce::OSCMessage message1 ("/test/message1", 5, 3.5f, juce::String ("hello"));
    juce::OSCMessage message2 ("/test/message2", 10, 7.0f, juce::String ("hello 2"));
    juce:: OSCBundle bundle;
    bundle.addElement (message1);
    bundle.addElement (message2);

    SECTION ("Copy construction and reference retrieval with OSCMessage")
    {
        osc::OSCMessageOrBundle mob (message1);

        CHECK_FALSE (mob.isBundle());
        CHECK_THROWS (mob.getBundle());

        CHECK (mob.isMessage());
        REQUIRE_NOTHROW (message1 == mob.getMessage());
    }

    SECTION ("Copy construction and reference retrieval with OSCBundle")
    {
        osc::OSCMessageOrBundle mob (bundle);

        CHECK_FALSE (mob.isMessage());
        CHECK_THROWS (mob.getMessage());

        CHECK (mob.isBundle());
        REQUIRE_NOTHROW (bundle == mob.getBundle());
    }

    SECTION ("Move construction and move-out retrieval with OSCMessage")
    {
        auto message1Copy = message1;
        osc::OSCMessageOrBundle mob (std::move (message1Copy));

        CHECK_FALSE (mob.isBundle());
        CHECK_THROWS (mob.getBundle());

        CHECK (mob.isMessage());
        CHECK_NOTHROW (message1 == mob.getMessage());
        REQUIRE_NOTHROW (message1 == juce::OSCMessage (std::move (mob)));
    }

    SECTION ("Move construction and move-out retrieval with OSCBundle")
    {
        auto bundleCopy = bundle;
        osc::OSCMessageOrBundle mob (std::move (bundleCopy));

        CHECK_FALSE (mob.isMessage());
        CHECK_THROWS (mob.getMessage());

        CHECK (mob.isBundle());
        CHECK_NOTHROW (bundle == mob.getBundle());
        REQUIRE_NOTHROW (bundle == juce::OSCBundle (std::move (mob)));
    }

}

} // namespace mrlab
