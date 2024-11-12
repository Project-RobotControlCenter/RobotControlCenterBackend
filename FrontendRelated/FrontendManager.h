//
// Created by elejandc on 11/12/24.
//

#ifndef FRONTENDMANAGER_H
#define FRONTENDMANAGER_H

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/websocket.hpp>
#include <iostream>
#include <boost/asio/steady_timer.hpp>

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace websocket = beast::websocket;
using tcp = asio::ip::tcp;

class FrontendManager {
public:
    FrontendManager(const FrontendManager&) = delete;
    FrontendManager& operator=(const FrontendManager&) = delete;
    ~FrontendManager();

    static bool initInstance(asio::io_context &ioc, unsigned short _frontend_connection_port) {
        if(!_instance) {
            _instance = std::unique_ptr<FrontendManager>(new FrontendManager(ioc, _frontend_connection_port));
        }
        return _instance != nullptr;
    }

    static FrontendManager& getInstance() {
        return *_instance;
    }

private:
    static std::unique_ptr<FrontendManager> _instance;
    asio::io_context &_ioc;

    FrontendManager(asio::io_context &ioc, unsigned short _frontend_connection_port);

    void onNewFrontendConnection(websocket::stream<tcp::socket> robot_websocket);
};



#endif //FRONTENDMANAGER_H
