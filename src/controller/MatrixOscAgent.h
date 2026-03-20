/*
    MatrixOscAgent.h

    Part of MR Lab Control Software

    SPDX-License-Identifier: EUPL-1.2
    SPDX-FileCopyrightText: Copyright (c) 2026 Martin Rumori/sonible GmbH
 */

#pragma once

#include "MatrixController.h"
#include "OscAgent.h"
#include <juce_events/juce_events.h>
#include <nlohmann/json.hpp>

namespace mrlab::controller
{

//==============================================================================
/** OSC agent for MatrixController.

    Provides an OSC interface from/to the JSON state tree of the
    Prodigy audio matrix. The implementation focuses on a
    bi-directional interface to items in the "settings" subgraph of
    the Prodigy's JSON state, as well as providing read-only access to
    the other state sub-trees.
 */
class MatrixOscAgent : public OscAgent,
                       public MatrixController::Listener,
                       private juce::Timer
{
public:
    //==============================================================================
    MatrixOscAgent (OscController& oscControllerIn, MatrixController& matrixControllerIn);
    ~MatrixOscAgent() override;

    // MatrixController::Listener interface
    void stateChanged (MatrixController&, MatrixController::State state) override;

    /** Handle an incoming Prodigy JSON control message from the matrix.

        @param msg Prodigy JSON message to handle.
     */
    void handleMatrixMessage (nlohmann::json msg);

private:
    //==============================================================================
    static constexpr auto prodigyPingIntervalMs = 5000;

    /** Send a ping to Prodigy matrix and warn on pending messages. */
    void timerCallback() override;

    /** Handle an incoming GET-RESP JSON message from the matrix. */
    void handleGetRespMessage (nlohmann::json&& msg);

    /** Handle an incoming UPDATE JSON message from the matrix. */
    void handleUpdateMessage (nlohmann::json&& msg);

    /** Handle an incoming ERROR JSON message from the matrix. */
    void handleErrorMessage (nlohmann::json&& msg);

    /** Handle an incoming ACK JSON message from the matrix. */
    void handleAckMessage (nlohmann::json&& msg);

    /** Create a Prodigy JSON control message for sending.

        This creates JSON messages for the Prodigy matrix control
        according to the Prodigy JSON procotol specification 1.0.

        @param type JSON control message type identifier.
        @param payload JSON value representing the message payload field.
        @param obj JSON value representing the optional obj pointer field.

        @return JSON value representing the Prodigy control message.

        @note The message will be assigned a sequence number, which
              will be added to the set of pending messages. It should
              be removed from the set when receiving the corresponding
              response (i.e., 'get_resp', 'ack' or 'error') using
              removeMessageFromPending().

        @see Type.
     */
    nlohmann::json makeMessage (std::string_view type, const nlohmann::json& payload = {}, const nlohmann::json& obj = {});

    /** Remove a Prodigy JSON control message from the set of pending messages.

        If the input message contains a 'seq' field, the corresponding
        sequence number will be removed from the set of pending
        messages.

        @param msg Message whose sequence number should be removed from pending.

        @return true on success, false if no sequence number in message
                or sequence number not in set of pending messages.
     */
    bool removeMessageFromPending (const nlohmann::json& msg);

    /** Create a one-argument OSC message from a JSON value.

        @param json Input JSON value.
        @return OSC message containing an argument corresponding to json.
        @throws ProdigyJsonTypeException.
     */
    static lo::Message json2osc (const nlohmann::json& json);

    /** Construct a Prodigy JSON state tree pointer from an OSC path.

        Splits the segments of the input OSC path and constructs an
        obj state pointer according to the Prodigy JSON procotol
        specification 1.0, section 2.2.8.2. To be used in Prodigy JSON
        messages like 'set' or 'get'.

        @param oscPath Input OSC path.
        @return JSON array representing the obj pointer.
     */
    static nlohmann::json oscPath2objPointer (std::string_view oscPath);

    /** Add an OSC method that will set a value in the Prodigy JSON state.

        Helper to register an OSC method (using OscAgent::addMethod())
        for a specified OSC path and type signature. The OSC method will

        @li retrieve the value from the incoming OSC message using
            the provided valueGetter function,
        @li assign it to the referenced JSON state value,
        @li send a corresponding 'set' JSON message to the Prodigy
            matrix and
        @li broadcast the state update to other OSC endpoints.

        @param oscPath OSC path to register the method for.
        @param oscTypes OSC type signature to listen for.
        @param state Reference to the state value that should be set.
        @param valueGetter Function to retrieve the value to be set
               from the incoming OSC message's argument vector.
     */
    template <std::invocable<lo_arg**, int> ValueGetter>
    void addStateSetterOscMethod (std::string_view oscPath, std::string_view oscTypes, nlohmann::json& state, ValueGetter&& valueGetter);

    /** Helper to invoke a function for primitive JSON values in a Prodigy state tree.

        Recursively traverses a given payload JSON tree (e.g., as
        received by a Prodigy JSON 'update' message) until a primitive
        JSON value is found (i.e., the leafs of the tree). The
        provided function is called for each primitive value with a
        const reference to that value in the input payload tree, a
        reference to the JSON value in the internally stored, full
        matrix state tree that corresponds to the input payload value,
        and the OSC path that may be used to address this value by OSC
        messages.

        @param func Function to be called on each primitive payload value.
        @param payload JSON value representing the payload tree.
        @param top JSON pointer into the internal matrix state tree
               that corresponds to the top-level of the payload tree
               (e.g., "/settings" in case payload only references the
               "settings" sub-tree of the matrix state).

        @throws ProdigyJsonStateException
     */
    template <std::invocable<const nlohmann::json&, nlohmann::json&, std::string_view> Func>
    void forEachPrimitiveInPayload (Func&& func, const nlohmann::json& payload, nlohmann::json::json_pointer top = nlohmann::json::json_pointer());

    //==============================================================================
    /** Prodigy JSON control message fields. */
    struct Field
    {
        static constexpr std::string_view type = "type";
        static constexpr std::string_view obj = "obj";
        static constexpr std::string_view payload = "payload";
        static constexpr std::string_view seq = "seq";
    };

    /** Prodigy JSON control message types. */
    struct Type
    {
        static constexpr std::string_view set = "set";
        static constexpr std::string_view update = "update";
        static constexpr std::string_view get = "get";
        static constexpr std::string_view get_resp = "get_resp";
        static constexpr std::string_view cmd = "cmd";
        static constexpr std::string_view ack = "ack";
        static constexpr std::string_view error = "error";
    };

    /** Prodigy JSON control message commands. */
    struct Command
    {
        static constexpr std::string_view flash = "flash";
        static constexpr std::string_view ping = "ping";
        static constexpr std::string_view reset_labels = "reset_labels";
        static constexpr std::string_view enable_auto_update = "enable_auto_update";
        static constexpr std::string_view disable_auto_update = "disable_auto_update";
        static constexpr std::string_view start_snapshot_recall = "start_snapshot_recall";
        static constexpr std::string_view stop_snapshot_recall = "stop_snapshot_recall";
    };

    //==============================================================================
    OscController& oscController;
    MatrixController& matrixController;
    nlohmann::json matrix;                   ///< Full state tree of the prodigy matrix.
    uint32_t sequence { 0 };                 ///< Next available message sequence number.
    std::unordered_set<uint32_t> pendingMsg; ///< Sequence numbers of pending messages.

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MatrixOscAgent)
};

} // namespace mrlab::controller
