//
// Created by elejandc on 11/10/24.
//

#include "RobotConnectionListener.h"

#include <iostream>

std::unique_ptr<RobotConnectionListener> RobotConnectionListener::_instance = nullptr;

RobotConnectionListener::RobotConnectionListener(asio::io_context &ioc, const asio::ip::tcp::endpoint &endpoint,
std::function<void(websocket::stream<tcp::socket>)> on_new_connection)
        : _acceptor(ioc), _socket(ioc), _on_new_robot_connection(std::move(on_new_connection)) {
    std::cout << "DEBUG : RobotConnectionListener - CONSTRUCTOR" << std::endl;

    beast::error_code ec;

    // Create the TCP acceptor
    try {
        _acceptor.open(endpoint.protocol(), ec);
        if(ec) throw std::runtime_error(ec.message());

        _acceptor.set_option(asio::socket_base::reuse_address(true), ec);
        if(ec) throw std::runtime_error(ec.message());

        _acceptor.bind(endpoint, ec);
        if(ec) throw std::runtime_error(ec.message());

        _acceptor.listen(asio::socket_base::max_listen_connections, ec);
        if(ec) throw std::runtime_error(ec.message());
    }catch (const std::exception &e) {
        std::cerr << "ERROR : Exception in RobotConnectionListener::RobotConnectionListener : " << e.what() << std::endl;
        return;
    }
}

RobotConnectionListener::~RobotConnectionListener() {
    std::cout << "DEBUG : RobotConnectionListener - DESTRUCTOR" << std::endl;
}

void RobotConnectionListener::runImp() {
    this->do_accept_tcp_connection();
}

void RobotConnectionListener::do_accept_tcp_connection() {
    // Zakładam, że _socket jest członkiem klasy, np. std::shared_ptr<tcp::socket> _socket;
    _acceptor.async_accept(
        _socket,
        [this](beast::error_code ec) {
            if (!ec) {
                // Obsługa zaakceptowanego połączenia
                std::cout << "DEBUG : RobotConnectionListener - do_accept_tcp_connection - tcp accepted" << std::endl;
                on_accept_tcp_connection(ec, std::move(_socket));
            } else {
                // Obsługa błędu
                std::cerr << "ERROR : Error accepting connection: " << ec.message() << std::endl;
            }
        });
}

void RobotConnectionListener::on_accept_tcp_connection(beast::error_code ec, tcp::socket socket) {
    if (ec) {
        std::cerr << "ERROR : Error accepting connection: " << ec.message() << std::endl;
        handle_error(ec, "Listener::on_accept_tcp_connection");
    } else {
        std::cout << "DEBUG : TCP CONNECTION ACCEPTED" << std::endl;
        auto websocket_stream = std::make_shared<websocket::stream<tcp::socket>>(std::move(socket));
        do_accept_websocket_connection(websocket_stream);
    }

    do_accept_tcp_connection();
}

void RobotConnectionListener::do_accept_websocket_connection(const std::shared_ptr<websocket::stream<tcp::socket>>& websocket_stream) {
    websocket_stream->async_accept(
        [this, websocket_stream](beast::error_code ec) {
            std::cout << "DEBUG : RobotConnectionListener - do_accept_websocket_connection - websocket accepted" << std::endl;
            on_accept_websocket_connection(ec, websocket_stream);
        });
}

void RobotConnectionListener::on_accept_websocket_connection(beast::error_code ec, const std::shared_ptr<websocket::stream<tcp::socket>>& websocket_stream) {
    if (ec) {
        std::cerr << "ERROR : Error accepting websocket connection: " << ec.message() << std::endl;
        handle_error(ec, "Listener::on_accept_websocket_connection");
        return;
    }
    _on_new_robot_connection(std::move(*websocket_stream));
}

void RobotConnectionListener::handle_error(const beast::error_code& ec, const std::string& context) {
    std::cerr << "ERROR : error in " << context << ": " << ec.message() << std::endl;
}