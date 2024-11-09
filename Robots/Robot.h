//
// Created by elejandc on 11/9/24.
//

#ifndef ROBOT_H
#define ROBOT_H
#include <string>


class Robot {
public:
    Robot(std::string db_id, std::string name, std::string mac_address, std::string ip, std::string port, bool isAccepted);
    ~Robot();
private:
    std::string _db_id;
    std::string _name;
    std::string _mac_address;
    std::string _ip;
    std::string _port;
    bool _isAccepted = false;
};



#endif //ROBOT_H
