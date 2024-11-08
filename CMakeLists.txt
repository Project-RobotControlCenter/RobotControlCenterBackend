cmake_minimum_required(VERSION 3.29)
project(RobotControlCenterBackend)

set(CMAKE_CXX_STANDARD 14)

add_executable(RobotControlCenterBackend main.cpp
        Validations/DataValidations.cpp
        Validations/DataValidations.h)
