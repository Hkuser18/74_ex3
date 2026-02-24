cmake_minimum_required(VERSION 4.0)
project(74_ex3)

set(CMAKE_CXX_STANDARD 14)

add_executable(74_ex3 main.cpp
        Ship.cpp
        Ship.h
        Cruiser.cpp
        Cruiser.h
        Freighter.cpp
        Freighter.h)
