//
// Created by elejandc on 11/14/24.
//

#ifndef SESSION_H
#define SESSION_H

#include <memory>
#include <string>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/websocket.hpp>
#include <iostream>
#include <unordered_map>
#include <boost/asio/steady_timer.hpp>

#include "Session.h"
#include "Robots/Robot.h"

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace websocket = beast::websocket;
using tcp = asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(asio::io_context &ioc,websocket::stream<tcp::socket> frontend_websocket);
    ~Session();

    void start();

private:
    bool _initialized = false;
    asio::io_context & _ioc;

    websocket::stream<tcp::socket> _frontend_websocket;
    beast::flat_buffer _frontend_input_buffer;

    std::shared_ptr<Robot> _robot;

    std::unordered_map<std::string, std::function<void()>> _actions;
    void initActions();

    void listenOnFrontend();
    void handleMessageFromFrontend();
};



#endif //SESSION_H
