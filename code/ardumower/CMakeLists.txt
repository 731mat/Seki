cmake_minimum_required(VERSION 3.6)
project(ardumower)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    adcman.cpp
    adcman.h
    ardumower.ino
    bt.cpp
    bt.h
    buzzer.cpp
    buzzer.h
    config.h
    drivers.cpp
    drivers.h
    due.cpp
    due.h
    DueTimer.cpp
    DueTimer.h
    flash_efc.cpp
    flash_efc.h
    flashmem.cpp
    flashmem.h
    gps.cpp
    gps.h
    imu.cpp
    imu.h
    mega.cpp
    mega.h
    mini.cpp
    mini.h
    mower.cpp
    mower.h
    NewPing.cpp
    NewPing.h
    perimeter.cpp
    perimeter.h
    pfod.cpp
    pfod.h
    pid.cpp
    pid.h
    pinman.cpp
    pinman.h
    robot.cpp
    robot.h)

add_executable(ardumower ${SOURCE_FILES})