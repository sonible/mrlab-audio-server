/*
    OSCOutputStream.h

    Part of MR Lab Control Software

    SPDX-License-Identifier: EUPL-1.2
    SPDX-FileCopyrightText: Copyright (c) 2025 Martin Rumori/sonible GmbH
 */

#pragma once

namespace mrlab::osc
{

//==============================================================================
/** Writes OSC data to an internal memory buffer, which grows as required.

    The data that was written into the stream can then be accessed later as
    a contiguous block of memory.

    This class implements the Open Sound Control 1.0 Specification for
    the format in which the OSC data will be written into the buffer.

    @note This class is copied from the internal implementation of
    juce::OSCSender and very slighty adjusted for using it as a public class.
 */
class OSCOutputStream
{
public:
    //==============================================================================
    /** Create an OSCOutputStream that uses an internal memory block. */
    OSCOutputStream() noexcept {}

    /** Create an OSCOutputStream that uses an externally owned memory block.

        @param memoryBlockToWriteTo Externally owned memory block.

        @note The destination block will always be larger than the amount of data
        that has been written to the stream, because the underlying MemoryOutputStream
        keeps some spare capacity at its end. To trim the block's size down to fit the
        actual data, call flush(), or delete tthe OSCOutputStream.

        @see juce::MemoryOutputStream (MemoryBlock&)
     */
    OSCOutputStream (MemoryBlock& memoryBlockToWriteTo) noexcept
        : output (memoryBlockToWriteTo, false)
    {}

    /** @returns a pointer to the data that has been written to the stream. */
    const void* getData() const noexcept { return output.getData(); }

    /** @returns the number of bytes of data that have been written to the stream. */
    size_t getDataSize() const noexcept { return output.getDataSize(); }

    /** @returns a std::span of the data that has been written to the stream. */
    std::span<const std::byte> asStdSpan() const noexcept { return { getData(), getDataSize() }; }

    /** Reset the stream and clear any data that has been written. */
    void reset() { output.reset(); }

    /** Resize the underlying memory block to the actual data.

        If the stream is writing to a user-supplied MemoryBlock, this will trim any excess
        capacity off the block, so that its length matches the amount of actual data that
        has been written so far.

        @see juce::MemoryOutputStream::flush()
     */
    void flush() { output.flush(); }

    //==============================================================================
    /** Write an OSCMessage to the stream.

        @param msg The OSCMessage to write.
        @return true on success, false on error.
     */
    bool writeMessage (const OSCMessage& msg)
    {
        if (! writeAddressPattern (msg.getAddressPattern()))
            return false;

        OSCTypeList typeList;

        for (auto& arg : msg)
            typeList.add (arg.getType());

        if (! writeTypeTagString (typeList))
            return false;

        for (auto& arg : msg)
            if (! writeArgument (arg))
                return false;

        return true;
    }

    /** Write an OSCBundle to the stream.

        @param msg The OSCBundle to write.
        @return true on success, false on error.
     */
    bool writeBundle (const OSCBundle& bundle)
    {
        if (! writeString ("#bundle"))
            return false;

        if (! writeTimeTag (bundle.getTimeTag()))
            return false;

        for (auto& element : bundle)
            if (! writeBundleElement (element))
                return false;

        return true;
    }

    /** Write an OSC bundle element to the stream, i.e., an OSCMessage or an OSCBundle.

        @param msg The bundle element to write.
        @return true on success, false on error.
     */
    bool writeBundleElement (const OSCBundle::Element& element)
    {
        const int64 startPos = output.getPosition();

        if (! writeInt32 (0)) // writing dummy value for element size
            return false;

        if (element.isBundle())
        {
            if (! writeBundle (element.getBundle()))
                return false;
        }
        else
        {
            if (! writeMessage (element.getMessage()))
                return false;
        }

        const int64 endPos = output.getPosition();
        const int64 elementSize = endPos - (startPos + 4);

        return output.setPosition (startPos) && writeInt32 ((int32) elementSize) && output.setPosition (endPos);
    }

private:
    //==============================================================================
    bool writeInt32 (int32 value)
    {
        return output.writeIntBigEndian (value);
    }

    bool writeUint64 (uint64 value)
    {
        return output.writeInt64BigEndian (int64 (value));
    }

    bool writeFloat32 (float value)
    {
        return output.writeFloatBigEndian (value);
    }

    bool writeString (const String& value)
    {
        if (! output.writeString (value))
            return false;

        const size_t numPaddingZeros = ~value.getNumBytesAsUTF8() & 3;

        return output.writeRepeatedByte ('\0', numPaddingZeros);
    }

    bool writeBlob (const MemoryBlock& blob)
    {
        if (! (output.writeIntBigEndian ((int) blob.getSize()) && output.write (blob.getData(), blob.getSize())))
            return false;

        const size_t numPaddingZeros = ~(blob.getSize() - 1) & 3;

        return output.writeRepeatedByte (0, numPaddingZeros);
    }

    bool writeColour (OSCColour colour)
    {
        return output.writeIntBigEndian ((int32) colour.toInt32());
    }

    bool writeTimeTag (OSCTimeTag timeTag)
    {
        return output.writeInt64BigEndian (int64 (timeTag.getRawTimeTag()));
    }

    bool writeAddress (const OSCAddress& address)
    {
        return writeString (address.toString());
    }

    bool writeAddressPattern (const OSCAddressPattern& ap)
    {
        return writeString (ap.toString());
    }

    bool writeTypeTagString (const OSCTypeList& typeList)
    {
        output.writeByte (',');

        if (typeList.size() > 0)
            output.write (typeList.begin(), (size_t) typeList.size());

        output.writeByte ('\0');

        size_t bytesWritten = (size_t) typeList.size() + 1;
        size_t numPaddingZeros = ~bytesWritten & 0x03;

        return output.writeRepeatedByte ('\0', numPaddingZeros);
    }

    bool writeArgument (const OSCArgument& arg)
    {
        switch (arg.getType())
        {
            case OSCTypes::int32:
                return writeInt32 (arg.getInt32());
            case OSCTypes::float32:
                return writeFloat32 (arg.getFloat32());
            case OSCTypes::string:
                return writeString (arg.getString());
            case OSCTypes::blob:
                return writeBlob (arg.getBlob());
            case OSCTypes::colour:
                return writeColour (arg.getColour());

            default:
                // In this very unlikely case you supplied an invalid OSCType!
                jassertfalse;
                return false;
        }
    }

    //==============================================================================
    MemoryOutputStream output;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSCOutputStream)
};

} // namespace mrlab::osc
