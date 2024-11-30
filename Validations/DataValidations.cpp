//
// Created by elejandc on 11/8/24.
//

#include "DataValidations.h"

#include <regex>

// Funkcja do sprawdzenia poprawności adresu IP
bool isValidIpAddress(const std::string& ip) {
    std::regex ip_pattern(
        R"((\d{1,3}\.){3}\d{1,3})"); // Prosty regex do sprawdzenia formatu IPv4
    if (std::regex_match(ip, ip_pattern)) {
        size_t start = 0;
        size_t end = 0;
        for (int i = 0; i < 4; ++i) {
            end = ip.find('.', start);
            const int part = std::stoi(ip.substr(start, end - start));
            if (part < 0 || part > 255) return false;
            start = end + 1;
        }
        return true;
    }
    return false;
}

// Funkcja do sprawdzenia poprawności portu
bool isValidPort(const std::string& port_str) {
    try {
        const int port = std::stoi(port_str);
        return port > 0 && port <= 65535;
    } catch (...) {
        return false;
    }
}

// Funkcja sprawdzająca poprawność argumentów
bool validateArguments(
    const std::string& local_network_db_ip,
    const std::string& local_network_db_port,
    const std::string& global_network_db_ip,
    const std::string& global_network_db_port,
    const std::string& db_password,
    const std::string& frontend_websocket_port,
    const std::string& robots_websocket_port) {

    if (!isValidIpAddress(local_network_db_ip)) {
        std::cerr << "ERROR : Invalid local network DB IP address: " << local_network_db_ip << "\n";
        return false;
    }
    if (!isValidPort(local_network_db_port)) {
        std::cerr << "ERROR : Invalid local network DB port: " << local_network_db_port << "\n";
        return false;
    }
    if (!isValidIpAddress(global_network_db_ip)) {
        std::cerr << "ERROR : Invalid global network DB IP address: " << global_network_db_ip << "\n";
        return false;
    }
    if (!isValidPort(global_network_db_port)) {
        std::cerr << "ERROR : Invalid global network DB port: " << global_network_db_port << "\n";
        return false;
    }
    if (db_password.empty()) {
        std::cerr << "ERROR : Database password cannot be empty.\n";
        return false;
    }
    if (!isValidPort(frontend_websocket_port)) {
        std::cerr << "ERROR : Invalid frontend websocket port: " << frontend_websocket_port << "\n";
        return false;
    }
    if (!isValidPort(robots_websocket_port)) {
        std::cerr << "ERROR : Invalid robots websocket port: " << robots_websocket_port << "\n";
        return false;
    }

    return true;
}