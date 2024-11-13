//
// Created by elejandc on 11/9/24.
//

#ifndef ROBOT_H
#define ROBOT_H
#include <string>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/websocket.hpp>
#include <iostream>
#include <boost/asio/steady_timer.hpp>
#include <utility>
#include <boost/json.hpp>

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace websocket = beast::websocket;
using tcp = asio::ip::tcp;
namespace json = boost::json;

class Robot {
public:
    // Robot() = default;
    Robot(websocket::stream<tcp::socket> &robot_websocket, asio::io_context& ioc, std::string db_id, std::string name, std::string mac_address, std::string ip, unsigned char port, bool isAccepted);
    ~Robot();

    std::string getDbId() {return _db_id;}
    std::string getName() {return _name;}
    std::string getMacAddress() {return _mac_address;}
    std::string getIp() {return _ip;}
    unsigned char getPort() const {return _port;}
    bool isAccepted() const {return _isAccepted;}

    void sendMessage(const json::value& message);

    void setOnReceivedMessageCallback(const std::function<void(const json::value&)> &callback) {_on_received_message_callback = callback;}

    void startReceivingMessages();

private:
    websocket::stream<tcp::socket> _robot_websocket;
    asio::io_context& _ioc;
    std::string _db_id;
    std::string _name;
    std::string _mac_address;
    std::string _ip;
    unsigned char _port;
    bool _isAccepted = false;
    std::function<void(const json::value&)> _on_received_message_callback;
    boost::beast::flat_buffer _buffer;

    void onReceive(boost::beast::error_code ec, std::size_t bytes_transferred);

};



#endif //ROBOT_H
