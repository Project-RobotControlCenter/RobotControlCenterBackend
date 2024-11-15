//
// Created by elejandc on 11/15/24.
//

#ifndef DATAPARSER_H
#define DATAPARSER_H
#include <boost/json.hpp>
#include <string>
#include <vector>
#include <iostream>
#include "MessageStructs.h"


class DataParser{
public:

    // FROM JSON TO STRUCT
    static st_getAllRobotsOrder parseJsonToStrucGetAllRobotsOrdert(const std::string& json);
    static st_connectToRobotOrder parseJsonToStructConnectToRobotOrder(const std::string& jsonString);

    // FROM STRUCT TO JSON
    static std::string parseStructToJson(const st_AllRobotsInfo& allRobotsInfo);

private:

    static boost::json::value toJson(const st_robotInfo& robotInfo);
};



#endif //DATAPARSER_H
