/*
    OSCHelpers.h

    Part of MR Lab Control Software

    SPDX-License-Identifier: EUPL-1.2
    SPDX-FileCopyrightText: Copyright (c) 2025 Martin Rumori/sonible GmbH
 */

#pragma once

#include <juce_osc/juce_osc.h>

namespace juce
{

/** Operator to test two OSCArgument objects for equality. */
inline JUCE_API bool JUCE_CALLTYPE operator== (const OSCArgument& lhs, const OSCArgument& rhs) noexcept
{
    const auto lhsType = lhs.getType();

    if (lhsType != rhs.getType())
        return false;

    if (lhsType == OSCTypes::int32)
        return lhs.getInt32() == rhs.getInt32();
    if (lhsType == OSCTypes::float32)
        return approximatelyEqual (lhs.getFloat32(), rhs.getFloat32());
    if (lhsType == OSCTypes::string)
        return lhs.getString() == rhs.getString();
    if (lhsType == OSCTypes::blob)
        return lhs.getBlob() == rhs.getBlob();
    if (lhsType == OSCTypes::colour)
        return lhs.getColour() == rhs.getColour();

    jassertfalse; // Unknown OSCArgument type!

    return false;
}

/** Operator to test two OSCArgument objects for inequality. */
inline JUCE_API bool JUCE_CALLTYPE operator!= (const OSCArgument& lhs, const OSCArgument& rhs) noexcept
{
    return ! (lhs == rhs);
}

/** Operator to test two OSCTimeTag objects for equality. */
inline JUCE_API bool JUCE_CALLTYPE operator== (const OSCTimeTag& lhs, const OSCTimeTag& rhs) noexcept
{
    return lhs.getRawTimeTag() == rhs.getRawTimeTag();
}

/** Operator to test two OSCTimeTag objects for inequality. */
inline JUCE_API bool JUCE_CALLTYPE operator!= (const OSCTimeTag& lhs, const OSCTimeTag& rhs) noexcept
{
    return ! (lhs == rhs);
}

/** Operator to test two OSCMessage objects for equality. */
inline JUCE_API bool JUCE_CALLTYPE operator== (const OSCMessage& lhs, const OSCMessage& rhs) noexcept
{
    return (lhs.getAddressPattern() == rhs.getAddressPattern() &&
            std::equal (lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
}

/** Operator to test two OSCMessage objects for inequality. */
inline JUCE_API bool JUCE_CALLTYPE operator!= (const OSCMessage& lhs, const OSCMessage& rhs) noexcept
{
    return ! (lhs == rhs);
}

// forward declaration
JUCE_API bool JUCE_CALLTYPE operator== (const OSCBundle::Element& lhs, const OSCBundle::Element& rhs) noexcept;

/** Operator to test two OSCBundle objects for equality. */
inline JUCE_API bool JUCE_CALLTYPE operator== (const OSCBundle& lhs, const OSCBundle& rhs) noexcept
{
    return (lhs.getTimeTag() == rhs.getTimeTag() &&
            std::equal (lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
}

/** Operator to test two OSCBundle objects for inequality. */
inline JUCE_API bool JUCE_CALLTYPE operator!= (const OSCBundle& lhs, const OSCBundle& rhs) noexcept
{
    return ! (lhs == rhs);
}

/** Operator to test two OSCBundle::Element objects for equality. */
inline JUCE_API bool JUCE_CALLTYPE operator== (const OSCBundle::Element& lhs, const OSCBundle::Element& rhs) noexcept
{
    const auto lhsIsMessage = lhs.isMessage();

    if (lhsIsMessage != rhs.isMessage())
        return false;

    if (lhsIsMessage)
        return lhs.getMessage() == rhs.getMessage();

    return lhs.getBundle() == rhs.getBundle();
}

/** Operator to test two OSCBundle::Element objects for inequality. */
inline JUCE_API bool JUCE_CALLTYPE operator!= (const OSCBundle::Element& lhs, const OSCBundle::Element& rhs) noexcept
{
    return ! (lhs == rhs);
}

} // namespace mrlab::osc
