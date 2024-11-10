//
// Created by elejandc on 11/10/24.
//

#ifndef ROBOTCONNECTIONLISTENER_H
#define ROBOTCONNECTIONLISTENER_H
#include <memory>
#include <string>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/websocket.hpp>

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace websocket = beast::websocket;
using tcp = asio::ip::tcp;

class RobotConnectionListener : public std::enable_shared_from_this<RobotConnectionListener> {
public:
    RobotConnectionListener(const RobotConnectionListener&) = delete;
    RobotConnectionListener& operator=(const RobotConnectionListener&) = delete;

    static bool initInstance(asio::io_context& ioc, const asio::ip::tcp::endpoint &endpoint, const std::function<void(websocket::stream<tcp::socket>)> &on_new_connection) {
        if(!_instance) {
            _instance = std::unique_ptr<RobotConnectionListener>(new RobotConnectionListener(ioc, endpoint, on_new_connection));
        }

        return _instance != nullptr;
    }

    static RobotConnectionListener* getInstance() {
        return _instance.get();
    }

    static void run() {getInstance()->runImp();}

private:
    static std::unique_ptr<RobotConnectionListener> _instance;
    asio::ip::tcp::acceptor _acceptor;
    std::function<void(websocket::stream<tcp::socket>)> _on_new_robot_connection;

    bool _is_running = false;

    RobotConnectionListener(asio::io_context& ioc, const asio::ip::tcp::endpoint &endpoint, const std::function<void(websocket::stream<tcp::socket>)> &on_new_connection);
    ~RobotConnectionListener();

    void runImp();
    void do_accept_tcp_connection();
    void on_accept_tcp_connection(beast::error_code ec);
    void do_accept_websocket_connection(const std::shared_ptr<websocket::stream<tcp::socket>>& websocket_stream);
    void on_accept_websocket_connection(beast::error_code ec, const std::shared_ptr<websocket::stream<tcp::socket>> &websocket_stream);

    void handle_error(const beast::error_code& ec, const std::string& context);
};



#endif //ROBOTCONNECTIONLISTENER_H
