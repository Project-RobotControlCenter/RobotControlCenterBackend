//
// Created by elejandc on 11/9/24.
//

#ifndef ROBOT_H
#define ROBOT_H
#include <string>


class Robot {
public:
    // Robot() = default;
    Robot(std::string db_id, std::string name, std::string mac_address, std::string ip, unsigned char port, bool isAccepted);
    ~Robot();

    std::string getDbId() {return _db_id;}
    std::string getName() {return _name;}
    std::string getMacAddress() {return _mac_address;}
    std::string getIp() {return _ip;}
    unsigned char getPort() const {return _port;}
    bool isAccepted() const {return _isAccepted;}

private:
    std::string _db_id;
    std::string _name;
    std::string _mac_address;
    std::string _ip;
    unsigned char _port;
    bool _isAccepted = false;
};



#endif //ROBOT_H
