cmake_minimum_required(VERSION 3.29)
project(RobotControlCenterBackend)

set(CMAKE_CXX_STANDARD 14)

add_executable(RobotControlCenterBackend main.cpp
        Validations/DataValidations.cpp
        Validations/DataValidations.h
        DatabaseRelated/DatabaseManager.cpp
        DatabaseRelated/DatabaseManager.h
        DatabaseRelated/DatabaseDAO.cpp
        DatabaseRelated/DatabaseDAO.h
        Robots/RobotManager.cpp
        Robots/RobotManager.h
        Robots/Robot.cpp
        Robots/Robot.h
        Robots/RobotConnectionListener.cpp
        Robots/RobotConnectionListener.h
        App.cpp
        App.h
        FrontendRelated/FrontendManager.cpp
        FrontendRelated/FrontendManager.h
        FrontendRelated/FrontendConnectionListener.cpp
        FrontendRelated/FrontendConnectionListener.h
        Session.cpp
        Session.h)

find_package(mongocxx REQUIRED)
find_package(bsoncxx REQUIRED)
find_package(Boost 1.75 REQUIRED COMPONENTS system thread json)

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

if (Boost_FOUND)
    message(STATUS "Boost found!")

    # Dodaj biblioteki do linkowania
    target_link_libraries(RobotControlCenterBackend
            Boost::system
            Boost::thread
            Boost::json
    )

    # Dodaj katalogi nagłówków
    target_include_directories(RobotControlCenterBackend PRIVATE /usr/local/include/boost)
else()
    message(FATAL_ERROR "Boost not found!")
endif()
