cmake_minimum_required(VERSION 3.16)
project(AuthClient LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network)

add_definitions(-DWIN32_LEAN_AND_MEAN)

add_executable(AuthClient
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    adminpanel.cpp
    adminpanel.h
    client.cpp
    client.h
)

target_link_libraries(AuthClient PRIVATE
    Qt6::Core
    Qt6::Widgets
    ws2_32
)
