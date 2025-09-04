# src/CMakeLists.txt

# Part of MR Lab Control Software

# SPDX-License-Identifier: EUPL-1.2
# SPDX-FileCopyrightText: Copyright (c) 2025 Martin Rumori/sonible GmbH

target_sources (mrlabctrl
    PRIVATE
        Main.cpp

        controller/MainController.h
        controller/MainController.cpp

        view/MainComponent.h
        view/MainComponent.cpp
        view/MainWindow.h
        view/MainWindow.cpp
)
