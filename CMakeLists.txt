cmake_minimum_required(VERSION 3.29)
project(RobotControlCenterBackend)

set(CMAKE_CXX_STANDARD 14)

add_executable(RobotControlCenterBackend main.cpp
        Validations/DataValidations.cpp
        Validations/DataValidations.h
        DatabaseRelated/DatabaseManager.cpp
        DatabaseRelated/DatabaseManager.h)

find_package(mongocxx REQUIRED)
find_package(bsoncxx REQUIRED)

if(mongocxx_FOUND)
    message(STATUS "mongocxx found!")
    target_link_libraries(RobotControlCenterBackend mongocxx)
    target_include_directories(RobotControlCenterBackend PRIVATE /usr/local/include/mongocxx/v_noabi)
else ()
    message(FATAL_ERROR "mongocxx not found!")
endif ()

if (bsoncxx_FOUND)
    message(STATUS "bsoncxx found!")
    target_link_libraries(RobotControlCenterBackend bsoncxx)
    target_include_directories(RobotControlCenterBackend PRIVATE /usr/local/include/bsoncxx/v_noabi)
else ()
    message(FATAL_ERROR "bsoncxx not found!")
endif ()
