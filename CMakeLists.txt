cmake_minimum_required(VERSION 3.0)
project(virtual_machine)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(vnvm
    Assembler.cpp
    Assembler.h
    Constants.cpp
    Constants.h
    main.cpp
    VirtualMachine.cpp
    VirtualMachine.h)
