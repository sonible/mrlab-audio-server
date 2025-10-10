# src/CMakeLists.txt

# Part of MR Lab Control Software

# SPDX-License-Identifier: EUPL-1.2
# SPDX-FileCopyrightText: Copyright (c) 2025 Martin Rumori/sonible GmbH

target_sources (mrlabctrl
    PRIVATE
        Main.cpp

        controller/AppConfigController.h
        controller/AppConfigController.cpp
        controller/AppController.h
        controller/AppController.cpp
        controller/AppHandle.h
        controller/AppHandle.cpp
        controller/MainController.h
        controller/MainController.cpp

        util/ListenerInterface.h

        view/AppControlComponent.h
        view/AppControlComponent.cpp
        view/MainComponent.h
        view/MainComponent.cpp
        view/MainWindow.h
        view/MainWindow.cpp
)
