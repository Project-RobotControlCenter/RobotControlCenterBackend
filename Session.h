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
#include <boost/asio/steady_timer.hpp>

#include "Session.h"

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
    websocket::stream<tcp::socket> _frontend_websocket;
    asio::io_context & _ioc;
    bool _initialized = false;

    beast::flat_buffer _frontend_input_buffer;

    void listenOnFrontend();
    void handleMessageFromFrontend();
};



#endif //SESSION_H
