cmake_minimum_required(VERSION 3.6)
project(ardumower)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    adcman.cpp
    adcman.h
        seki-arduino.ino
    bt.cpp
    bt.h
    buzzer.cpp
    buzzer.h
    config.h
    drivers.cpp
    drivers.h
        robot.cpp
    robot.h Seki.cpp Seki.h)

add_executable(ardumower ${SOURCE_FILES})