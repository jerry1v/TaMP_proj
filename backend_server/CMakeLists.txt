cmake_minimum_required(VERSION 3.16)

project(backend_server)

set(CMAKE_CXX_STANDARD 17)

add_executable(backend_server
    main.cpp
    databasemanager.cpp
    aes.cpp
    sha1.cpp
    newton.cpp
    stego.cpp
)

target_link_libraries(backend_server ws2_32)