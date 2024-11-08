#include <iostream>

#include "Validations/DataValidations.h"

// Local_network_db_ip Local_network_db_port Global_network_db_ip Global_network_db_port db_password frontend_websocket_port robots_websocket_port

#define REQUESTED_ARGUMENTS 8

int main(int argc, const char * argv[])
{
    std::cout << "INFO : RobotControlCenterBackend - START" << std::endl;

    if(argc != REQUESTED_ARGUMENTS) {
        std::cerr << "ERROR : Wrong number of arguments" << std::endl;
        std::cerr << "ERROR : expected: RobotControlCenterBackend Local_network_db_ip Local_network_db_port Global_network_db_ip Global_network_db_port db_password frontend_websocket_port robots_websocket_port" << std::endl;
        return 1;
    } else {
        std::cout << "INFO : Arguments OK" << std::endl;
    }

    if (validateArguments(argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], argv[7])) {
        std::cout << "INFO : All arguments are valid.\n";
    } else {
        std::cerr << "ERROR : Invalid arguments provided.\n";
        return 1;
    }

    std::cout << "INFO : RobotControlCenterBackend - END" << std::endl;
    return 0;
}
