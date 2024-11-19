//
// Created by elejandc on 11/14/24.
//

#include "Session.h"

#include "Message_processing/DataParser.h"
#include "Robots/Robot.h"
#include "Robots/RobotManager.h"

Session::Session(const unsigned int session_id, asio::io_context &ioc,websocket::stream<tcp::socket> frontend_websocket, std::function<void(unsigned int)> on_session_end)
    : _session_id(session_id), _ioc(ioc), _frontend_websocket(std::move(frontend_websocket)), _on_session_end(std::move(on_session_end)) {
    std::cout << "INFO : Session - Constructor" << std::endl;
    initActions();
}

Session::~Session() {
    std::cout << "INFO : Session - Destructor" << std::endl;

    if (_frontend_websocket.is_open()) {
        _frontend_websocket.close(websocket::close_code::normal);
    }

    if (_robot) {
        _robot.reset();
    }
}

void Session::start() {
    if(_initialized) return;
    _initialized = true;
    std::cout << "INFO : Session - Start" << std::endl;

    _frontend_websocket.set_option(websocket::stream_base::timeout::suggested(beast::role_type::server));

    listenOnFrontend();
}

void Session::initActions() {
    _actions["getAllRobots"] = [](const std::shared_ptr<Session> &session) {
        std::cout << "INFO: getAllRobots action invoked" << std::endl;

        std::vector<st_robotInfo> robots_info = RobotManager::getAllRobotsData();

        st_AllRobotsInfo allRobotsInfo;
        allRobotsInfo.message_type = "AllRobotsInfo";
        allRobotsInfo.data.robots_info = std::move(robots_info);

        std::string json_response = DataParser::parseStructToJson(allRobotsInfo);

        session->_frontend_websocket.text(true);
        session->_frontend_websocket.write(asio::buffer(json_response));

        std::cout << "INFO: Sent all robots data response : " << json_response << "" << std::endl;
    };

    _actions["connectToRobot"] = [](const std::shared_ptr<Session> &session) {
        std::cout << "INFO: connectToRobotO action invoked" << std::endl;

        std::string message_response = beast::buffers_to_string(session->_frontend_input_buffer.data());

        st_connectToRobotOrder order = DataParser::parseJsonToStructConnectToRobotOrder(message_response);

        std::shared_ptr<Robot> robot = RobotManager::getRobot(order.data.mac_address);
        if (robot) {
            // std::cout << "INFO: Robot with MAC " << order.data.mac_address << " is already connected." << std::endl;
            std::cout << "INFO: Connect session with robot with MAC " << order.data.mac_address << std::endl;
            session->_robot = robot;
            // TODO: TESTw
            session->_robot->setOnDisconnectToSessionCallback(std::bind(&Session::handleRobotDisconnected, session));
            session->_robot->setOnReceivedTextMessageCallback(std::bind(&Session::handleTextMessageFromRobot, session, std::placeholders::_1));
            session->_robot->setOnReceivedBinaryMessageCallback(std::bind(&Session::handleBinaryMessageFromRobot, session, std::placeholders::_1));
            //
            session->_robot->startReceivingMessages();
        } else {
            std::cerr << "ERROR: No robot found with MAC address " << order.data.mac_address << std::endl;
        }

        session->_frontend_input_buffer.consume(session->_frontend_input_buffer.size());
    };

    _actions["disconnectFromRobot"] = [](const std::shared_ptr<Session> &session) {
        std::cout << "INFO: disconnectFromRobot action invoked" << std::endl;

        if(session->_robot) {
            std::cout << "INFO: Disconnect session with robot with MAC " << session->_robot->getMacAddress() << std::endl;
            session->_robot.reset();

            session->_frontend_websocket.text(true);
            session->_frontend_websocket.write(asio::buffer(R"({"message_type":"RobotDisconnected"})"));
        } else {
            session->_frontend_websocket.text(true);
            session->_frontend_websocket.write(asio::buffer(R"({"message_type":"NoRobotConnected"})"));

            std::cerr << "ERROR: No robot connected" << std::endl;
        }

        session->_frontend_input_buffer.consume(session->_frontend_input_buffer.size());
    };
}

void Session::listenOnFrontend() {
    _frontend_websocket.async_read(_frontend_input_buffer, [this] (beast::error_code ec, std::size_t bytes_transferred) {
        if(ec) {
            std::cerr << "ERROR : Session - Error while reading from frontend websocket: " << ec.message() << std::endl;

            if(ec == websocket::error::closed) {
                std::cout << "INFO : Session - Frontend websocket closed" << std::endl;
                this->closeSession(ec.message());
                return;
            }

            if(ec == boost::asio::error::operation_aborted) {
                std::cout << "INFO : Session - Frontend websocket operation aborted" << std::endl;
                this->closeSession(ec.message());
                return;
            }

            this->listenOnFrontend();
            return;
        }
        this->handleTextMessageFromFrontend();
        this->_frontend_input_buffer.consume(this->_frontend_input_buffer.size());

        //Repeat
        this->listenOnFrontend();
    });
}

void Session::handleTextMessageFromFrontend() {
    std::cout << "INFO : Session - Received message from frontend" << std::endl;
    std::string message_response = beast::buffers_to_string(_frontend_input_buffer.data());
    std::cout << "INFO : Session - message from frontend : " << message_response << std::endl;

    std::string message_type = DataParser::getMessageTypeFromJson(message_response);

    if(message_type == "robotControl") {
        std::cout << "INFO : redirected message from frontend to robot" << std::endl;
        _robot->sendMessage(_frontend_input_buffer.data());
        return;
    }

    if (_actions.find(message_type) != _actions.end()) {
        std::cout << "INFO : Session - Action found for message type " << message_type << std::endl;
        std::shared_ptr<Session> session = shared_from_this();
        _actions[message_type](session);
        return;
    }

    std::cout << "INFO : Session - No action found for message type " << message_type << std::endl;
}

// void Session::listenOnRobot() {
//     _robot->setOnReceivedTextMessageCallback(std::bind(&Session::handleMessageFromRobot, this, std::placeholders::_1));
// }

void Session::handleTextMessageFromRobot(const json::value &message) {
    std::cout << "INFO : Session - Received message from robot" << std::endl;
    std::string message_response = json::serialize(message);
    std::cout << "INFO : Session - message from robot : " << message_response << std::endl;

    _frontend_websocket.binary(false);

    std::string message_type = DataParser::getMessageTypeFromJson(message_response);

    // if(message_type == "dataStreamFromRobot") {
    //     _frontend_websocket.async_write(asio::buffer(message_response), [this] (beast::error_code ec, std::size_t bytes_transferred) {
    //         if(ec) {
    //             std::cerr << "ERROR : Session - Error while sending message to frontend: " << ec.message() << std::endl;
    //             this->closeSession(ec.message());
    //             return;
    //         }
    //         std::cout << "INFO : redirected message from robot to frontend" << std::endl;
    //     });
    // }

    if (_actions.find(message_type) != _actions.end()) {
        std::cout << "INFO : Session - Action found for message type " << message_type << std::endl;
        std::shared_ptr<Session> session = shared_from_this();
        _actions[message_type](session);
        return;
    }

    std::cout << "INFO : Session - No action found for message type " << message_type << std::endl;
}

void Session::handleBinaryMessageFromRobot(const asio::streambuf &buffer) {
    _frontend_websocket.binary(true);

    auto buffers = buffer.data();

    _frontend_websocket.async_write(buffers, [this] (beast::error_code ec, std::size_t bytes_transferred) {
            if(ec) {
                std::cerr << "ERROR : Session - Error while sending binary message to frontend: " << ec.message() << std::endl;
                this->closeSession(ec.message());
                return;
            }
            std::cout << "INFO : redirected message from robot to frontend" << std::endl;
        });
}

void Session::handleRobotDisconnected() {
    std::cout << "INFO : Session - Robot disconnected" << std::endl;

    if(_robot) {
        _robot->setOnDisconnectToSessionCallback(nullptr);
        _robot->setOnReceivedTextMessageCallback(nullptr);
        _robot.reset();
        _robot = nullptr;
    }
}

void Session::closeSession(const std::string &reason) {
    std::cout << "INFO : Session - Closing session : " << reason << "" << std::endl;

    this->_on_session_end(_session_id);
}

