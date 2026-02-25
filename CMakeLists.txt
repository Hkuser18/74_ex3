cmake_minimum_required(VERSION 3.14)
project(74_ex3)

set(CMAKE_CXX_STANDARD 14)

add_executable(74_ex3 main.cpp
        Sim_object.cpp
        Sim_object.h
        Ship.cpp
        Ship.h
        Cruiser.cpp
        Cruiser.h
        Freighter.cpp
        Freighter.h
        Patrol.cpp
        Patrol.h
        Port.cpp
        Port.h
        Model.cpp
        Model.h)
