//
// Created by elejandc on 11/10/24.
//

#include "RobotConnectionListener.h"

#include <iostream>

RobotConnectionListener::RobotConnectionListener(asio::io_context &ioc, const asio::ip::tcp::endpoint &endpoint,
                                                 const std::function<void(websocket::stream<tcp::socket>)> &on_new_connection)
        : _acceptor(ioc, endpoint), _on_new_robot_connection(on_new_connection) {
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

}

void RobotConnectionListener::runImp() {
    this->do_accept_tcp_connection();
}

void RobotConnectionListener::do_accept_tcp_connection() {
    auto socket = std::make_shared<tcp::socket>(_acceptor.get_executor().context());
    _acceptor.async_accept(
            *socket,
            beast::bind_front_handler(
                &RobotConnectionListener::on_accept_tcp_connection,
                shared_from_this()));
}

void RobotConnectionListener::on_accept_tcp_connection(beast::error_code ec) {
    if (ec) {
        handle_error(ec, "Listener::on_accept_tcp_connection");
    } else {
        auto websocket_stream = std::make_shared<websocket::stream<tcp::socket>>(std::move(*socket));
        do_accept_websocket_connection(websocket_stream);
    }

    this->do_accept_tcp_connection();
}

void RobotConnectionListener::do_accept_websocket_connection(const std::shared_ptr<websocket::stream<tcp::socket>>& websocket_stream) {
    websocket_stream->async_accept(
        beast::bind_front_handler(
            &RobotConnectionListener::on_accept_websocket_connection,
            shared_from_this(),
            websocket_stream));
}

void RobotConnectionListener::on_accept_websocket_connection(beast::error_code ec, const std::shared_ptr<websocket::stream<tcp::socket>> &websocket_stream) {
    if (ec) {
        handle_error(ec, "Listener::on_accept_websocket_connection");
        return;
    }
    _on_new_robot_connection(std::move(*websocket_stream));
}

void RobotConnectionListener::handle_error(const beast::error_code& ec, const std::string& context) {
    std::cerr << "ERROR : error in " << context << ": " << ec.message() << std::endl;
}