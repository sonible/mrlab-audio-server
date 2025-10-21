# CMakeLists.txt

# Part of MR Lab Control Software

# SPDX-License-Identifier: EUPL-1.2
# SPDX-FileCopyrightText: Copyright (c) 2025 Martin Rumori/sonible GmbH

cmake_minimum_required (VERSION 3.16)

# This setting has to occur BEFORE the project call to work correctly
set (CMAKE_OSX_DEPLOYMENT_TARGET "10.14" CACHE STRING "macOS deployment target" FORCE)

project (mrlab VERSION 0.0.1)

# CPM dependency management: https://github.com/cpm-cmake/CPM.cmake
include (cmake/CPM.cmake)

# Compile commands, useful for some IDEs like VS-Code
set (CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Static linkage on Windows
set (CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# JUCE dependency
CPMAddPackage ("gh:juce-framework/JUCE#8.0.10")

# civetweb for webserver and websocket
CPMAddPackage (
    URI "gh:civetweb/civetweb#dfdfdb6926765541751cab055472f70daa9aa2dd"
    OPTIONS
    "CIVETWEB_ENABLE_SERVER_EXECUTABLE OFF"
    "CIVETWEB_BUILD_TESTING OFF"
    "CIVETWEB_ENABLE_ASAN OFF"
    "CIVETWEB_ENABLE_CXX ON"
    "CIVETWEB_ENABLE_WEBSOCKETS ON"
)

#====================================================================================
set (MRLABCTRL_TARGET mrlabctrl)

juce_add_gui_app (${MRLABCTRL_TARGET}
    PRODUCT_NAME  mrlabctrl
    COMPANY_NAME  sonible
    BUNDLE_ID     com.sonible.mrlabctrl

    HARDENED_RUNTIME_ENABLED TRUE
    NEEDS_WEBVIEW2 TRUE
)

# We do not intend to use JuceHeader.h generally,
# this is just for exploiting its ProjectInfo:: facility.
juce_generate_juce_header (${MRLABCTRL_TARGET})

add_subdirectory (resources)
add_subdirectory (src)

target_include_directories (${MRLABCTRL_TARGET} PUBLIC "src")

target_compile_definitions (${MRLABCTRL_TARGET}
    PRIVATE
        DONT_SET_USING_JUCE_NAMESPACE=1
        JUCE_WEB_BROWSER=1
        JUCE_USE_WIN_WEBVIEW2_WITH_STATIC_LINKING=1
        JUCE_USE_CURL=0
)

target_compile_features (${MRLABCTRL_TARGET} PUBLIC cxx_std_20)

target_link_libraries (${MRLABCTRL_TARGET}
    PRIVATE
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
        juce::juce_gui_extra
        civetweb-cpp
)
