/*
    OSCPacket.h

    Part of MR Lab Control Software

    SPDX-License-Identifier: EUPL-1.2
    SPDX-FileCopyrightText: Copyright (c) 2025 Martin Rumori/sonible GmbH
 */

#pragma once

namespace mrlab::osc
{

//==============================================================================
/** Facility for the binary serialisation of an OSC bundle or message.

    Serialises and deserialises OSC entities (i.e., bundles or messages) to/from
    their binary representation according to the OSC 1.0 spec, suitable for
    transmission over frame/datagram based transports (e.g., UDP). Specific
    encoding/decoding for stream-based transport (e.g., SLIP for TCP) is not
    included.

    An OSCPacket is constructed with either an OSC entity (message or bundle)
    object for serialisation or with a memory reference for deserialisation.
    The implementation guarantees that the contained OSC entity and its
    serialised representation are always in sync.

    Advanced functionality includes partial deserialisation of OSC messages
    (OSC address and argument vector) for efficient re-serialisation with a
    different OSC address, e.g., in message relay scenarios.

 */
class OSCPacket
{
public:
    //==============================================================================
    /** Construct an OSCPacket from an OSC entity (message or bundle).

        The OSC entity will be serialised
        Memory for deserialisation will be allocated internally.
     */
    template <std::convertible_to<OSCMessageOrBundle> OSCEntity>
    OSCPacket (OSCEntity&& oscEntity)
        : messageOrBundle (std::forward<OSCEntity> (oscEntity)),
          outData (outDataInternal)
    {}

    /** Construct an OSCPacket from an OSC entity (message or bundle).

        Deserialisation will use the provided non-owned memory block.
     */
    template <std::convertible_to<OSCMessageOrBundle> OSCEntity>
    OSCPacket (OSCEntity&& oscEntity, juce::MemoryBlock& memoryBlock)
        : messageOrBundle (std::forward<OSCEntity> (oscEntity)),
          outData (memoryBlock)
    {}

    /** Construct an OSCPacket for deserialisation from a data view.

        If reserialisation is performed, internal memory will be allocated.
     */
    OSCPacket (std::span<const std::byte> serialisedData)
        : inData (std::move (serialisedData)),
          outData (outDataInternal)
    {}

    /** Construct an OSCPacket for deserialisation from a data view.

        If reserialisation is performed, the provided non-owned memory block
        will be used.
     */
    OSCPacket (std::span<const std::byte> serialisedData,
               juce::MemoryBlock& memoryBlock)
        : inData (std::move (serialisedData)),
          outData (memoryBlock)
    {}

    /** Construct an OSCPacket for deserialisation from a memory block.

        If re-serialisation is performed, the same memory block will be used
        (i.e., it will be overwritten).
     */
    OSCPacket (juce::MemoryBlock& memoryBlock)
        : inData ({ memoryBlock.getData(), memoryBlock.getDataSize() }),
          outData (memoryBlock)
    {}

    /** Set the OSC entity (message or bundle) for serialisation.

        @param oscEntity OSC message or bundle to serialise.
     */
    template <std::convertible_to<OSCMessageOrBundle> OSCEntity>
    void setMessageOrBundle (OSCEntity&& oscEntity);

    /** Get a view to the serialised packet data.

        @returns std::span that points to the serialised data.
     */
    std::span<const std::byte> getSerialised() const;


private:
    //==============================================================================
    /** Serialise the current messageOrBundle member to outData. */
    void serialise();

    /** Parse (deserialise) the current inData to an OSC entity (message or bundle). */
    void parse();

    //==============================================================================
    OSCMessageOrBundle messageOrBundle; ///< Deserialised OSC entity represented by this.

    std::span<const std::byte> inData; ///< Non-owned memory for de-serialisation.
    juce::MemoryBlock& outData; ///< Reference to the serialisation memory being used.
    juce::MemoryBlock outDataInternal; ///< Owned memory for serialisation.

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSCPacket)
};

} // namespace mrlab::osc
