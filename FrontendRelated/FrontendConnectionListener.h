//
// Created by elejandc on 11/12/24.
//

#ifndef FRONTENDCONNECTIONLISTENER_H
#define FRONTENDCONNECTIONLISTENER_H

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/websocket.hpp>

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace websocket = beast::websocket;
using tcp = asio::ip::tcp;

class FrontendConnectionListener {
public:
    FrontendConnectionListener(const FrontendConnectionListener&) = delete;
    FrontendConnectionListener& operator=(const FrontendConnectionListener&) = delete;
    ~FrontendConnectionListener();

    static bool initInstance(asio::io_context& ioc, const asio::ip::tcp::endpoint &endpoint, const std::function<void(websocket::stream<tcp::socket>)> &on_new_connection) {
        if(!_instance) {
            _instance = std::unique_ptr<FrontendConnectionListener>(new FrontendConnectionListener(ioc, endpoint, on_new_connection));
        }

        return _instance != nullptr;
    }

    static FrontendConnectionListener* getInstance() {
        return _instance.get();
    }

    static void run() {getInstance()->runImp();}

private:
    static std::unique_ptr<FrontendConnectionListener> _instance;
    asio::ip::tcp::acceptor _acceptor;
    asio::ip::tcp::socket _socket;
    std::function<void(websocket::stream<tcp::socket>)> _on_new_frontend_connection;

    bool _is_running = false;

    FrontendConnectionListener(asio::io_context& ioc, const asio::ip::tcp::endpoint &endpoint, std::function<void(websocket::stream<tcp::socket>)> on_new_connection);

    void runImp();
    void do_accept_tcp_connection();
    void on_accept_tcp_connection(beast::error_code ec, tcp::socket socket);
    void do_accept_websocket_connection(const std::shared_ptr<websocket::stream<tcp::socket>>& websocket_stream);
    void on_accept_websocket_connection(beast::error_code ec, const std::shared_ptr<websocket::stream<tcp::socket>>& websocket_stream);

    void handle_error(const beast::error_code& ec, const std::string& context);
};



#endif //FRONTENDCONNECTIONLISTENER_H
