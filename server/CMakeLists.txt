cmake_minimum_required(VERSION 3.5)

project(Server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(Server main.cpp
    server.cpp
    server.h
    ../socket/socket.cpp
    ../socket/socket.h
    connhandler.cpp
    connhandler.h)
