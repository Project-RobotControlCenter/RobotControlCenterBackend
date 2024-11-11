//
// Created by elejandc on 11/9/24.
//

#include "RobotManager.h"
#include "RobotConnectionListener.h"
#include <boost/json.hpp>

std::unique_ptr<RobotManager> RobotManager::_instance = nullptr;

RobotManager::RobotManager(asio::io_context &ioc, unsigned short _robot_connection_port)
    : _ioc(ioc){
    std::cout << "DEBUG : RobotManager - CONSTRUCTOR" << std::endl;

    // Inicjalizacja Listenera
    if(RobotConnectionListener::initInstance(_ioc, asio::ip::tcp::endpoint(asio::ip::address_v4::any(), _robot_connection_port), std::bind(&RobotManager::onNewRobotConnection, this, std::placeholders::_1))) {
        std::cout << "INFO : RobotManager - Listener initialized" << std::endl;
        RobotConnectionListener::run();
    } else {
        std::cerr << "ERROR : RobotManager - Listener initialization failed" << std::endl;
    }
}

RobotManager::~RobotManager() {
    std::cout << "DEBUG : RobotManager - DESTRUCTOR" << std::endl;
}

void RobotManager::onNewRobotConnection(websocket::stream<tcp::socket> robot_websocket) {
    try {
        // Przygotowanie i wysłanie zapytania JSON
        boost::json::object request_message;
        request_message["message_type"] = "info_request";
        std::string request = boost::json::serialize(request_message);
        robot_websocket.text(true);
        robot_websocket.write(asio::buffer(request));

        asio::steady_timer timer(_ioc);
        timer.expires_after(std::chrono::seconds(5));

        bool response_received = false;
        std::string response_message;

        auto buffer = asio::dynamic_buffer(response_message);
        robot_websocket.async_read(buffer, [&](beast::error_code ec, std::size_t bytes_transferred) {
            timer.cancel();
            if (!ec) {
                response_received = true;
                boost::json::value parsed_response = boost::json::parse(response_message);
                if (parsed_response.is_object() && parsed_response.as_object()["message_type"] == "robot_info") {
                    const auto& data = parsed_response.as_object()["data"].as_object();
                    std::string mac_address = data.at("mac_address").as_string().c_str();
                    std::string internal_robot_name = data.at("internal_robot_name").as_string().c_str();
                    _robots[mac_address] = Robot(mac_address, internal_robot_name, "", "", "", false);
                    std::cout << "Robot added: " << internal_robot_name << " (" << mac_address << ")" << std::endl;
                } else {
                    std::cerr << "Invalid response format from robot." << std::endl;
                }
            } else {
                std::cerr << "Error receiving response: " << ec.message() << std::endl;
            }
        });

        timer.async_wait([&](const beast::error_code &ec) {
            if (!ec && !response_received) {
                robot_websocket.close(websocket::close_code::normal);
                std::cerr << "No response from robot within 5 seconds. Disconnected." << std::endl;
            }
        });
    } catch (const std::exception &e) {
        std::cerr << "Exception in RobotManager::onNewRobotConnection: " << e.what() << std::endl;
    }
}
