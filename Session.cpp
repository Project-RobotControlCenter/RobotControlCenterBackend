//
// Created by elejandc on 11/14/24.
//

#include "Session.h"

Session::Session(asio::io_context &ioc,websocket::stream<tcp::socket> frontend_websocket)
    : _ioc(ioc), _frontend_websocket(std::move(frontend_websocket)) {
    std::cout << "INFO : Session - Constructor" << std::endl;
}

Session::~Session() {
    std::cout << "INFO : Session - Destructor" << std::endl;
}
