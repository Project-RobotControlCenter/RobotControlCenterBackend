//
// Created by elejandc on 11/14/24.
//

#ifndef MESSAGESTRUCTS_H
#define MESSAGESTRUCTS_H
#include <string>
#include <vector>

// OTHERS

struct st_robotInfo {
    std::string mac_address;
    std::string robot_name;
    std::string robot_ip;
    unsigned char robot_port;
};

// INPUTS

struct st_connectToRobotOrder {
    std::string message_type;
    struct {
        std::string mac_address;
    }data;
};

struct st_getAllRobotsOrder {
    std::string message_type;
    struct {
        //
    }data;
};

// OUTPUTS

struct st_AllRobotsInfo {
    std::string message_type;
    struct {
        std::vector<st_robotInfo> robots_info;
    } data;
};

#endif //MESSAGESTRUCTS_H
