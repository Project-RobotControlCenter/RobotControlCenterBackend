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
    Robot(websocket::stream<tcp::socket> &robot_websocket, asio::io_context &ioc, std::string db_id, std::string name,
          std::string mac_address, std::string ip, unsigned char port, bool isAccepted,
          const std::function<void(const std::string &)> &on_disconnect_callback);
    ~Robot();

    std::string getDbId() {return _db_id;}
    std::string getName() {return _name;}
    std::string getMacAddress() {return _mac_address;}
    std::string getIp() {return _ip;}
    unsigned char getPort() const {return _port;}
    bool isAccepted() const {return _isAccepted;}

    void sendMessage(const json::value& message);
    void sendMessage(const beast::flat_buffer::const_buffers_type& buffer);

    void setOnReceivedTextMessageCallback(const std::function<void(const json::value&)> &callback) {_on_received_text_message_callback = callback;}
    void setOnReceivedBinaryMessageCallback(const std::function<void(beast::flat_buffer&)> &callback) {_on_received_binary_message_callback = callback;}
    void setOnDisconnectToSessionCallback(const std::function<void(void)> &callback) {_on_disconnect_to_session_callback = callback;}

    void startReceivingMessages() {
        _readMessages = true;
        startReceivingMessagesImp();
    };

    void stopReceivingMessages() {_readMessages = false;}

private:
    websocket::stream<tcp::socket> _robot_websocket;
    asio::io_context& _ioc;
    std::string _db_id;
    std::string _name;
    std::string _mac_address;
    std::string _ip;
    unsigned char _port;
    bool _isAccepted = false;
    bool _readMessages = false;
    std::function<void(const json::value&)> _on_received_text_message_callback;
    std::function<void(beast::flat_buffer&)> _on_received_binary_message_callback;
    std::function<void(const std::string&)> _on_disconnect_to_manager_callback;
    std::function<void(void)> _on_disconnect_to_session_callback;
    beast::flat_buffer _buffer;
    // asio::streambuf _buffer;

    void onReceive(boost::beast::error_code ec, std::size_t bytes_transferred);
    void startReceivingMessagesImp();
};



#endif //ROBOT_H
