//
// Created by elejandc on 11/9/24.
//

#include "Robot.h"

#include <utility>

Robot::Robot(websocket::stream<tcp::socket> &robot_websocket, asio::io_context& ioc, std::string db_id, std::string name, std::string mac_address, std::string ip, unsigned char port, bool isAccepted)
    : _robot_websocket(std::move(robot_websocket)), _ioc(ioc), _db_id(std::move(db_id)), _name(std::move(name)), _mac_address(std::move(mac_address)), _ip(std::move(ip)), _port(port), _isAccepted(isAccepted)
{

}

Robot::~Robot() {
}

void Robot::sendMessage(const json::value &message) {
    try {
        std::string serialized_message = json::serialize(message);
        _robot_websocket.text(true); // Set the WebSocket message mode to text
        _robot_websocket.write(boost::asio::buffer(serialized_message));
        std::cout << "Sent message: " << serialized_message << std::endl;
    } catch (const boost::system::system_error& se) {
        std::cerr << "Error while sending message: " << se.what() << std::endl;
    }
}

void Robot::startReceivingMessages() {
    _robot_websocket.async_read(
       _buffer,
       [this](boost::beast::error_code ec, std::size_t bytes_transferred) {
           onReceive(ec, bytes_transferred);
       });
    _ioc.run();
}

void Robot::onReceive(boost::beast::error_code ec, std::size_t bytes_transferred) {
    if (ec) {
        std::cerr << "ERROR :Error while receiving message: " << ec.message() << std::endl;
        return;
    }

    try {
        std::string receivedMessage = boost::beast::buffers_to_string(_buffer.data());
        _buffer.consume(_buffer.size()); // Clear the buffer after reading

        json::value parsed_message = json::parse(receivedMessage);
        std::cout << "Received message: " << receivedMessage << std::endl;

        // Trigger the callback if set
        if (_on_received_message_callback) {
            _on_received_message_callback(parsed_message);
        }
    } catch (const json::system_error& je) {
        std::cerr << "ERROR : Error parsing JSON message: " << je.what() << std::endl;
    }

    // Continue receiving messages asynchronously
    startReceivingMessages();
}
