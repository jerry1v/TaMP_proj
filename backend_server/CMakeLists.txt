cmake_minimum_required(VERSION 3.16)
project(AuthServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DWIN32_LEAN_AND_MEAN)

add_executable(AuthServer
    main.cpp
    databasemanager.cpp
    databasemanager.h
    aes.cpp
    aes.h
    sha1.cpp
    sha1.h
    newton.cpp
    newton.h
    stego.cpp
    stego.h
)

target_link_libraries(AuthServer PRIVATE ws2_32)
