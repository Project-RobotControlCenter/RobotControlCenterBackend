//
// Created by elejandc on 11/9/24.
//

#include "RobotManager.h"

#include "RobotConnectionListener.h"
#include "../DatabaseRelated/DatabaseDAO.h"

#include <thread>
#include <boost/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>



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

std::shared_ptr<Robot> RobotManager::getRobotImp(const std::string& mac_address) {
    const auto it = _robots.find(mac_address);
    if (it != _robots.end() && it->second) {
        if (it->second.use_count() == 1) {
            return it->second;
        }
    }
    return nullptr;
}

void RobotManager::onNewRobotConnection(websocket::stream<tcp::socket> robot_websocket) {
    std::cout << "INFO : RobotManager - New robot connected" << std::endl;

    try {
        // Get additional information from the robot (like mac address)
        boost::json::object request_message;
        request_message["message_type"] = "info_request";
        std::string request = boost::json::serialize(request_message);
        robot_websocket.text(true);
        robot_websocket.write(asio::buffer(request));

        // Set timeout for response
        auto timer = std::make_shared<asio::steady_timer>(_ioc);
        timer->expires_after(std::chrono::seconds(5));

        auto buffer = std::make_shared<asio::streambuf>();
        auto response_message = std::make_shared<std::string>();

        // Start asynchronous read
        robot_websocket.async_read(*buffer, [this, timer, buffer, response_message, &robot_websocket](beast::error_code ec, std::size_t bytes_transferred) {
            timer->cancel();

            if (ec) {
                std::cerr << "DEBUG : Can't get additional info from robot. EC: " << ec.message() << std::endl;
                return;
            }

            // Use buffers_to_string to convert buffer content to string
            *response_message = beast::buffers_to_string(buffer->data());
            std::cout << "response_message : " << *response_message << std::endl;

            boost::json::value parsed_response = boost::json::parse(*response_message);

            // Handle parsed response as needed
            if (!parsed_response.is_object()) {
                std::cout << "DEBUG: can't pars to json" << std::endl;
                return;
            }

            boost::json::object& obj = parsed_response.as_object();

            if(obj.contains("message_type") && obj["message_type"].as_string() != "robot_info") {
                std::cout << "DEBUG : receive wrong type of message_type. expected : robot_info. receive : " << obj["message_type"].as_string().c_str() << std::endl;
                return;
            }

            std::string mac_address;
            std::string internal_robot_name;

            if(obj.contains("data")) {
                boost::json::value& data = obj["data"];

                if(data.is_object()) {
                    boost::json::object& data_obj = data.as_object();

                    if(data_obj.contains("mac_address")) {
                        mac_address = data_obj["mac_address"].as_string().c_str();
                    } else std::cout << "break 3" << std::endl;

                    if(data_obj.contains("internal_robot_name")) {
                        internal_robot_name = data_obj["internal_robot_name"].as_string().c_str();
                    }  else std::cout << "break 4" << std::endl;
                } else std::cout << "break 2" << std::endl;
            } else std::cout << "break 1" << std::endl;

            if(mac_address.empty() || internal_robot_name.empty()) {
                std::cout << "DEBUG : read : | mac_address : " << mac_address << " | internal_robot_name : " << internal_robot_name << std::endl;
                return;
            }

            //Check if robot is already on the list
            if(_robots.find(mac_address) != _robots.end()) {
                std::cout << "Robot already on the _robots list" << std::endl;
                return;
            }

            //Get other information
            auto& socket = robot_websocket.next_layer();
            const tcp::endpoint remote_endpoint = socket.remote_endpoint();
            const std::string ip = remote_endpoint.address().to_string();
            const unsigned short port = remote_endpoint.port();

            //Add to _robots
            std::cout << "Add robot" << std::endl;
            auto result = _robots.emplace(mac_address, std::make_shared<Robot>(robot_websocket, _ioc, "", internal_robot_name, mac_address, ip, port, false));
            if (!result.second) {
                std::cerr << "ERROR: Robot with MAC address " << mac_address << " already exists or failed to add." << std::endl;
                return; // Handle the error case appropriately
            }
            Robot * robot = result.first->second.get(); // Reference to the newly added robot

            std::cout << "bp 1" << std::endl;

            // Add this robot to database
            bsoncxx::builder::stream::document document_builder;
            document_builder << "name" << robot->getName()
                             << "mac_address" << robot->getMacAddress()
                             << "ip" << robot->getIp()
                             << "port" << static_cast<int>(robot->getPort())  // Ensure type compatibility
                             << "isAccepted" << robot->isAccepted();

            bsoncxx::document::value document_value = document_builder << bsoncxx::builder::stream::finalize;

            std::cout << "bp 2" << std::endl;

            // Insert into database using DatabaseDAO
            if (DatabaseDAO::insertDocument("Robots", document_value.view())) {
                std::cout << "INFO : Robot successfully added to the database." << std::endl;
            } else {
                std::cerr << "ERROR : Failed to add Robot to the database." << std::endl;
            }
        });

        // Run the IO context to process asynchronous events
        _ioc.run();

    } catch (const std::exception &e) {
        std::cerr << "ERROR : Exception in RobotManager::onNewRobotConnection: " << e.what() << std::endl;
        return;
    }
}
