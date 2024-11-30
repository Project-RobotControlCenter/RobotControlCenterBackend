//
// Created by elejandc on 11/9/24.
//

#include "Robot.h"

#include <utility>

Robot::Robot(websocket::stream<tcp::socket> &robot_websocket, asio::io_context& ioc, std::string db_id, std::string name, std::string mac_address, std::string ip, unsigned char port, bool isAccepted, const std::function<void(const std::string&)> &on_disconnect_to_manager_callback)
    : _robot_websocket(std::move(robot_websocket)), _ioc(ioc), _db_id(std::move(db_id)), _name(std::move(name)), _mac_address(std::move(mac_address)), _ip(std::move(ip)), _port(port), _isAccepted(isAccepted), _on_disconnect_to_manager_callback(on_disconnect_to_manager_callback)
{
    std::cout << "INFO : ROBOT - Constructor" << std::endl;
}

Robot::~Robot() {
    std::cout << "INFO : ROBOT - Destructor" << std::endl;
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

void Robot::sendMessage(const beast::flat_buffer::const_buffers_type& buffer) {
    try {
        // Konwersja bufora na string
        std::string serialized_message(boost::asio::buffer_cast<const char*>(buffer), boost::asio::buffer_size(buffer));

        _robot_websocket.text(true); // Ustawienie trybu wiadomości WebSocket na tekst
        _robot_websocket.write(boost::asio::buffer(serialized_message));
        std::cout << "Sent message: " << serialized_message << std::endl;
    } catch (const boost::system::system_error& se) {
        std::cerr << "Error while sending message: " << se.what() << std::endl;
    }
}

void Robot::startReceivingMessagesImp() {
    std::cout << "INFO : Robot - Starting to receive messages" << std::endl;
    _robot_websocket.async_read(
       _buffer,
       [this](boost::beast::error_code ec, std::size_t bytes_transferred) {
           std::cout << "INFO : Robot - Received message" << std::endl;
           onReceive(ec, bytes_transferred);
       });
}

void Robot::onReceive(boost::beast::error_code ec, std::size_t bytes_transferred) {
    if (ec) {
        std::cerr << "ERROR : Error while receiving message: " << ec.message() << std::endl;

        if(ec == websocket::error::closed) {
            std::cout << "INFO : Robot - WebSocket connection closed" << std::endl;
            _on_disconnect_to_manager_callback(_mac_address);
            if(_on_disconnect_to_session_callback) _on_disconnect_to_session_callback();
            _readMessages = false;
            return;
        }

        if(ec == boost::asio::error::operation_aborted) {
            std::cout << "INFO : Robot - WebSocket operation aborted" << std::endl;
            _on_disconnect_to_manager_callback(_mac_address);
            if(_on_disconnect_to_session_callback) _on_disconnect_to_session_callback();
            _readMessages = false;
            return;
        }

        return;
    }

    try {
        if(_robot_websocket.got_text()) {
            //TEXT DATA

            std::string receivedMessage = boost::beast::buffers_to_string(_buffer.data());
            _buffer.consume(_buffer.size()); // Clear the buffer after reading

            json::value parsed_message = json::parse(receivedMessage);
            std::cout << "Received message: " << receivedMessage << std::endl;

            // Trigger the callback if set
            if (_on_received_text_message_callback) {
                _on_received_text_message_callback(parsed_message);
            }
        }else {
            //BINARY DATA
            if(_on_received_binary_message_callback) {
                _on_received_binary_message_callback(_buffer);
            }

            _buffer.consume(_buffer.size());
        }
    } catch (const json::system_error& je) {
        std::cerr << "ERROR : Error parsing JSON message: " << je.what() << std::endl;
    }

    // Continue receiving messages asynchronously
    if(_readMessages) {
        startReceivingMessages();
    }
}
