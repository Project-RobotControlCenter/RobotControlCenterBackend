//
// Created by elejandc on 11/14/24.
//

#include "Session.h"

#include "Message_processing/DataParser.h"
#include "Robots/Robot.h"
#include "Robots/RobotManager.h"

Session::Session(asio::io_context &ioc,websocket::stream<tcp::socket> frontend_websocket)
    : _ioc(ioc), _frontend_websocket(std::move(frontend_websocket)) {
    std::cout << "INFO : Session - Constructor" << std::endl;
    initActions();
}

Session::~Session() {
    std::cout << "INFO : Session - Destructor" << std::endl;
}

void Session::start() {
    if(_initialized) return;
    _initialized = true;
    std::cout << "INFO : Session - Start" << std::endl;

    _frontend_websocket.set_option(websocket::stream_base::timeout::suggested(beast::role_type::server));

    listenOnFrontend();
}

void Session::initActions() {
    _actions["getAllRobots"] = [this]() {
        std::cout << "INFO: getAllRobots action invoked" << std::endl;

        std::vector<st_robotInfo> robots_info = RobotManager::getAllRobotsData();

        st_AllRobotsInfo allRobotsInfo;
        allRobotsInfo.message_type = "AllRobotsInfo";
        allRobotsInfo.data.robots_info = std::move(robots_info);

        std::string json_response = DataParser::parseStructToJson(allRobotsInfo);

        _frontend_websocket.text(true);
        _frontend_websocket.write(asio::buffer(json_response));

        std::cout << "INFO: Sent all robots data response" << std::endl;
    };

    _actions["connectToRobot"] = [this]() {
        std::cout << "INFO: connectToRobotO action invoked" << std::endl;

        std::string message_response = beast::buffers_to_string(_frontend_input_buffer.data());

        st_connectToRobotOrder order = DataParser::parseJsonToStructConnectToRobotOrder(message_response);

        std::shared_ptr<Robot> robot = RobotManager::getRobot(order.data.mac_address);
        if (robot) {
            std::cout << "INFO: Robot with MAC " << order.data.mac_address << " is already connected." << std::endl;
            _robot = robot;
        } else {
            std::cerr << "ERROR: No robot found with MAC address " << order.data.mac_address << std::endl;
        }

        _frontend_input_buffer.consume(_frontend_input_buffer.size());
    };
}

void Session::listenOnFrontend() {
    _frontend_websocket.async_read(_frontend_input_buffer, [self = shared_from_this()] (beast::error_code ec, std::size_t bytes_transferred) {
        if(ec) {
            std::cerr << "ERROR : Session - Error while reading from frontend websocket: " << ec.message() << std::endl;
            self->listenOnFrontend();
            return;
        }
        self->handleMessageFromFrontend();
        self->_frontend_input_buffer.consume(self->_frontend_input_buffer.size());

        //Repeat
        self->listenOnFrontend();
    });
}

void Session::handleMessageFromFrontend() {
    std::cout << "INFO : Session - Received message from frontend" << std::endl;
    std::string message_response = beast::buffers_to_string(_frontend_input_buffer.data());
    std::cout << "INFO : Session - message from frontend : " << message_response << std::endl;

    std::string message_type = DataParser::getMessageTypeFromJson(message_response);

    if (_actions.find(message_type) != _actions.end()) {
        std::cout << "INFO : Session - Action found for message type " << message_type << std::endl;
        _actions[message_type]();
    } else {
        std::cout << "INFO : Session - No action found for message type " << message_type << std::endl;
    }
}

