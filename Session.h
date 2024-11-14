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

class Session {
public:
    Session(asio::io_context &ioc,websocket::stream<tcp::socket> frontend_websocket);
    ~Session();

private:
    websocket::stream<tcp::socket> _frontend_websocket;
    asio::io_context & _ioc;
};



#endif //SESSION_H
