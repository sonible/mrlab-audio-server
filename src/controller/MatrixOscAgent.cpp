/*
    MatrixOscAgent.cpp

    Part of MR Lab Control Software

    SPDX-License-Identifier: EUPL-1.2
    SPDX-FileCopyrightText: Copyright (c) 2026 Martin Rumori/sonible GmbH
 */

// For std::numeric_limits<>::min/max on Windows.
#ifndef NOMINMAX
#define NOMINMAX 1
#endif

#include "MatrixOscAgent.h"
#include <util/Logger.h>
#include <lo/lo_cpp.h>
#include <Exceptions.h>
#include <charconv>

namespace mrlab::controller
{

MatrixOscAgent::MatrixOscAgent (OscController& oscControllerIn, MatrixController& matrixControllerIn)
    : OscAgent (oscControllerIn),
      oscController (oscControllerIn),
      matrixController (matrixControllerIn)
{
    matrixController.addListener (this);
}

MatrixOscAgent::~MatrixOscAgent()
{
    matrixController.removeListener (this);
}

void MatrixOscAgent::stateChanged (MatrixController&, MatrixController::State state)
{
    // Query full JSON state tree from matrix after connecting.
    if (state == MatrixController::State::connected)
    {
        startTimer (prodigyPingIntervalMs);
        matrixController.sendMatrixMessage (makeMessage (Type::get));
    }
}

void MatrixOscAgent::handleMatrixMessage (nlohmann::json msg)
{
#if JUCE_DEBUG
    std::cout << "[JSON Prodigy recv]" << std::endl;
    std::cout << std::setw (2) << msg << std::endl;
#endif

    try
    {
        const auto& type = msg.at (Field::type).get_ref<const std::string&>();

        if (type == Type::get_resp)
            return handleGetRespMessage (std::move (msg));
        if (type == Type::update)
            return handleUpdateMessage (std::move (msg));
        if (type == Type::error)
            return handleErrorMessage (std::move (msg));
        if (type == Type::ack)
            return handleAckMessage (std::move (msg));

        Logger::logError ("MatrixOscAgent: Received unknown Prodigy JSON message with type: " + type);
    }
    catch (const std::runtime_error& e)
    {
        return Logger::logError (juce::String ("MatrixOscAgent: Failure to deal with incoming Prodigy JSON message: ") + e.what());
    }
}

void MatrixOscAgent::timerCallback()
{
    if (auto numPending = pendingMsg.size(); numPending > 0)
        Logger::logWarn (juce::String ("MatrixOscAgent: ") + juce::String (numPending) + " Prodigy JSON command message responses pending.");

    matrixController.sendMatrixMessage (makeMessage (Type::cmd, Command::ping));
}

void MatrixOscAgent::handleGetRespMessage (nlohmann::json&& msg)
{
    jassert (msg.at (Field::type).get_ref<const std::string&>() == Type::get_resp);

    removeMessageFromPending (msg);

    if (msg.contains (Field::obj) && ! msg.at (Field::obj).empty())
        return Logger::logError ("MatrixOscAgent: Received unsupported get_resp Prodigy JSON message with obj pointer: " + msg.at (Field::obj).dump());

    if (! matrix.is_null())
        return Logger::logError ("MatrixOscAgent: Received full state get_resp Prodigy JSON message but state has been already stored.");

    // Store full Prodigy state.
    matrix = msg.at (Field::payload);

    // Add state getter OSC methods for entire tree.
    auto addGetter = [this] (const nlohmann::json& /*payload*/, nlohmann::json& state, std::string_view oscPath) {

        addMethod (oscPath, {}, [this, &state] (OscEndpoint* source, std::string_view path, const lo::Message&) {
            if (! source)
                return Logger::logWarn (std::string ("MatrixOscAgent: Discarding state query message without sender for OSC path: ") += path);

            try
            {
                send (*source, path, json2osc (state));
            }
            catch (const ProdigyJsonTypeException& e)
            {
                Logger::logError (juce::String ("MatrixOscAgent: Cannot respond to state query (") + e.what() + ").");
            }
        });
    };

    try
    {
        forEachPrimitiveInPayload (std::move (addGetter), matrix);
    }
    catch (const ProdigyJsonStateException& e)
    {
        Logger::logError (juce::String ("MatrixOscAgent: Error initialising the Prodigy state (") + e.what() + ").");
        return;
    }

    // Add state setter OSC methods for /settings subtree.
    auto addSetter = [this] (const nlohmann::json& /*payload*/, nlohmann::json& state, std::string_view oscPath) {
        // Add OSC method corresponding to the type of the JSON state value.
        if (state.is_number_integer())
            return addStateSetterOscMethod (oscPath, "i", state, [] (lo_arg** argv, int) { return argv[0]->i; });

        if (state.is_number_float())
            return addStateSetterOscMethod (oscPath, "f", state, [] (lo_arg** argv, int) { return argv[0]->f; });

        if (state.is_string())
            return addStateSetterOscMethod (oscPath, "s", state, [] (lo_arg** argv, int) { return &argv[0]->s; });

        // Support multiple ways of setting boolean state values.
        if (state.is_boolean())
        {
            addStateSetterOscMethod (oscPath, "T", state, [] (lo_arg**, int) { return true; });
            addStateSetterOscMethod (oscPath, "F", state, [] (lo_arg**, int) { return false; });

            // Note: int message will be coerced to float by liblo.
            addStateSetterOscMethod (oscPath, "f", state, [] (lo_arg** argv, int) { return std::abs (argv[0]->f) > std::numeric_limits<float>::min(); });

            addStateSetterOscMethod (oscPath, "s", state, [] (lo_arg** argv, int) {
                std::string_view val (&argv[0]->s);
                return val == "true";
            });

            return;
        }

        throw ProdigyJsonStateException (std::string ("Unknown primitive type of JSON state value at ") += oscPath);
    };

    try
    {
        forEachPrimitiveInPayload (std::move (addSetter), matrix.at ("settings"), nlohmann::json::json_pointer ("/settings"));
    }
    catch (const ProdigyJsonStateException& e)
    {
        Logger::logError (juce::String ("MatrixOscAgent: Error initialising the Prodigy state (") + e.what() + ").");
    }
}

void MatrixOscAgent::handleUpdateMessage (nlohmann::json&& msg)
{
    jassert (msg.at (Field::type).get_ref<const std::string&>() == Type::update);

    auto update = [this] (const nlohmann::json& payload, nlohmann::json& state, std::string_view oscPath) {
        // Update local state from payload.
        state = payload;

        // Broadcast update to OSC clients.
        try
        {
            broadcast (oscPath, json2osc (state));
        }
        catch (const std::runtime_error& e)
        {
            Logger::logError ("MatrixOscAgent: Error sending OSC for path " + std::string (oscPath) + " (" + e.what() + ").");
        }
    };

    try
    {
        forEachPrimitiveInPayload (std::move (update), msg.at (Field::payload));
    }
    catch (const ProdigyJsonStateException& e)
    {
        Logger::logError (juce::String ("MatrixOscAgent: Error processing Prodigy JSON 'update' message (") + e.what() + ").");
    }
}

void MatrixOscAgent::handleErrorMessage (nlohmann::json&& msg)
{
    jassert (msg.at (Field::type).get_ref<const std::string&>() == Type::error);

    removeMessageFromPending (msg);

    return Logger::logError (juce::String ("MatrixOscAgent: Received Prodigy JSON error for message seq ") +
        (msg.contains (Field::seq) ? nlohmann::to_string (msg.at (Field::seq)) : "<?>") + " (" + msg.at (Field::obj).get_ref<std::string&>() + ").");
}

void MatrixOscAgent::handleAckMessage (nlohmann::json&& msg)
{
    jassert (msg.at (Field::type).get_ref<const std::string&>() == Type::ack);

    removeMessageFromPending (msg);

#if JUCE_DEBUG
    return Logger::logInfo (juce::String ("MatrixOscAgent: Received Prodigy JSON ack for message seq: ") +
        (msg.contains (Field::seq) ? nlohmann::to_string (msg.at (Field::seq)) : "<?>"));
#endif
}

nlohmann::json MatrixOscAgent::makeMessage (std::string_view type, const nlohmann::json& payload, const nlohmann::json& obj)
{
    nlohmann::json msg;

    pendingMsg.insert (sequence);
    msg[Field::seq] = sequence++;

    msg[Field::type] = type;

    if (! payload.is_null())
        msg[Field::payload] = payload;

    if (! obj.is_null())
        msg[Field::obj] = obj;

    return msg;
}

bool MatrixOscAgent::removeMessageFromPending (const nlohmann::json& msg)
{
    if (! msg.contains (Field::seq))
        return false;

    if (const auto& seq = msg.at (Field::seq); seq.is_number_unsigned())
        return bool (pendingMsg.erase (seq.get<uint32_t>()));

    return false;
}

lo::Message MatrixOscAgent::json2osc (const nlohmann::json& json)
{
    if (json.is_number_integer())
        return osc::Message (json.get<int32_t>());
    if (json.is_number_float())
        return osc::Message (json.get<float>());
    if (json.is_string())
        return osc::Message (json.get<std::string_view>());
    if (json.is_boolean())
        return osc::Message (int32_t (json.get<bool>()));

    throw ProdigyJsonTypeException (std::string ("json2osc: cannot create OSC message from value with type ") += json.type_name());
}

nlohmann::json MatrixOscAgent::oscPath2objPointer (std::string_view oscPath)
{
    using std::operator""sv;

    // Strip "/matrix/".
    jassert (oscPath.starts_with (osc::Address::matrix));
    oscPath.remove_prefix (osc::Address::matrix.size() + 1); // +1 for trailing /

    // Split OSC path and build 'obj' JSON array.
    nlohmann::json obj;

    for (const auto seg : std::views::split (oscPath, "/"sv))
    {
        const auto segment = std::string_view (seg.data(), seg.size());

        // Try to parse the segment as a number for array indexing, otherwise use as object key.
        uint32_t index;
        auto* last = segment.data() + segment.size();
        auto result = std::from_chars (segment.data(), last, index);

        if (result.ptr == last)
            obj.emplace_back (index);
        else
            obj.emplace_back (segment);
    }

    return obj;
}

template <std::invocable<lo_arg**, int> ValueGetter>
void MatrixOscAgent::addStateSetterOscMethod (std::string_view oscPath, std::string_view oscTypes, nlohmann::json& state, ValueGetter&& valueGetter)
{
    addMethod (oscPath, oscTypes, [this, &state, obj = oscPath2objPointer (oscPath), getter = std::forward<ValueGetter> (valueGetter)] (OscEndpoint* source, std::string_view path, std::string_view /*types*/, lo_arg** argv, int argc, const lo::Message& msg) {
        state = getter (argv, argc); // Assign new value to state.
        auto setMsg = makeMessage (Type::set, state, obj); // Prodigy JSON set message.

#if JUCE_DEBUG
        std::cout << "[JSON Prodigy send]" << std::endl;
        std::cout << std::setw (2) << setMsg << std::endl;
#endif

        matrixController.sendMatrixMessage (std::move (setMsg));

        // Propagate state change to other endpoints.
        broadcast (path, msg, source);
    });
}

template <std::invocable<const nlohmann::json&, nlohmann::json&, std::string_view> Func>
void MatrixOscAgent::forEachPrimitiveInPayload (Func&& func, const nlohmann::json& payload, nlohmann::json::json_pointer top)
{
    /* Helper for recursive state traversal.

       @param p Current reference into input payload.
       @param s Current reference into state, corresponding to p.
       @param o Recursively built OSC path matching the current payload/state references.
       @param r Recurse function (passed on).
     */
    auto recurse = [f = std::forward<Func> (func)] (const nlohmann::json& p, nlohmann::json& s, std::string_view o, auto&& r) -> void {

        if (p.is_primitive() && s.is_primitive())
        {
            f (p, s, o);
            return;
        }

        if (p.is_object() && s.is_object())
        {
            for (const auto& [key, value] : p.items())
            {
                if (! s.contains (key))
                    throw ProdigyJsonStateException (std::string ("MatrixOscAgent::forEachPrimitiveInPayload: Payload references unknown key in matrix state at ") += o);

                std::string path { o };
                (path += '/') += key;
                r (value, s.at (key), path, r);
            }
            return;
        }

        if (p.is_array() && s.is_array())
        {
            for (const auto& elem : p)
            {
                /* According to the Prodigy array semantics (Prodigy
                   JSON specification 2.2.8.1), we expect inner arrays
                   with [index, value] here.
                 */
                if (! elem.is_array() || elem.empty() || ! elem[0].is_number_unsigned())
                    throw ProdigyJsonStateException (std::string ("MatrixOscAgent::forEachPrimitiveInPayload: Payload array does not contain indexed sub-arrays at ") += o);

                const auto index = elem[0].get<uint32_t>();

                // Search for inner array with matching index in state.
                auto subArrayMatches = [index, o] (const auto& sub) {
                    if (! sub.is_array() || sub.empty() || ! sub[0].is_number_unsigned())
                        throw ProdigyJsonStateException (std::string ("MatrixOscAgent::forEachPrimitiveInPayload: State array does not contain indexed sub-arrays at ") += o);

                    return sub[0].template get<uint32_t>() == index;
                };

                if (const auto sub = std::find_if (s.begin(), s.end(), std::move (subArrayMatches)); sub != s.end())
                {
                    std::string path { o };
                    (path += '/') += std::to_string (index);
                    r (elem[1], (*sub)[1], path, r);
                    continue;
                }

                throw ProdigyJsonStateException (std::string ("MatrixOscAgent::forEachPrimitiveInPayload: No matching state array for payload array index ") + std::to_string (index) + " at " += o);
            }
            return;
        }

        throw ProdigyJsonStateException (std::string ("MatrixOscAgent::forEachPrimitiveInPayload: Unsupported JSON value type in payload at ") += o);
    };

    // Prepare initial OSC path.
    auto path = top.to_string();
    path.insert (0, osc::Address::matrix);

    if (! matrix.contains (top))
        throw ProdigyJsonStateException (std::string ("MatrixOscAgent::forEachPrimitiveInPayload: Cannot access sub-state pointed to by top at ") += top.to_string());

    recurse (payload, matrix.at (top), path, recurse);
}

} // namespace mrlab::controller
