//
// Created by elejandc on 11/8/24.
//

#ifndef DATAVALIDATIONS_H
#define DATAVALIDATIONS_H

#include <string>
#include <iostream>


bool isValidIpAddress(const std::string& ip);

bool isValidPort(const std::string& port_str);

bool validateArguments(
    const std::string& local_network_db_ip,
    const std::string& local_network_db_port,
    const std::string& global_network_db_ip,
    const std::string& global_network_db_port,
    const std::string& db_password,
    const std::string& frontend_websocket_port,
    const std::string& robots_websocket_port);

#endif //DATAVALIDATIONS_H
