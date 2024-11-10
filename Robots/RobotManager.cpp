//
// Created by elejandc on 11/9/24.
//

#include "RobotManager.h"
#include "RobotConnectionListener.h"


std::unique_ptr<RobotManager> RobotManager::_instance = nullptr;

RobotManager::RobotManager(asio::io_context &ioc, unsigned short _robot_connection_port) {
    // Init Robot Connection Listener
    RobotConnectionListener::initInstance(ioc, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), _robot_connection_port), std::bind(&RobotManager::onNewRobotConnection, this, std::placeholders::_1));
    RobotConnectionListener::run();
}

RobotManager::~RobotManager() {

}

void RobotManager::onNewRobotConnection(websocket::stream<tcp::socket> robot_websocket) {
    try {
        // Przygotowanie i wysłanie zapytania JSON
        json::object request_message;
        request_message["message_type"] = "info_request";
        std::string request = json::serialize(request_message);
        robot_websocket.text(true);
        robot_websocket.write(asio::buffer(request));

        // Utworzenie timera do monitorowania odpowiedzi (5-sekundowy timeout)
        asio::steady_timer timer(robot_websocket.get_executor().context());
        timer.expires_after(std::chrono::seconds(5));

        bool response_received = false;
        std::string response_message;

        // Rozpoczęcie asynchronicznego odbioru odpowiedzi
        robot_websocket.async_read(
            asio::dynamic_buffer(response_message),
            [&](beast::error_code ec, std::size_t bytes_transferred) {
                timer.cancel(); // Anuluj timer, jeśli odpowiedź przyszła
                if (!ec) {
                    response_received = true;
                    // Odbiór zakończony pomyślnie
                    json::value parsed_response = json::parse(response_message);
                    if (parsed_response.is_object() && parsed_response.as_object()["message_type"] == "robot_info") {
                        const auto& data = parsed_response.as_object()["data"].as_object();
                        std::string mac_address = data["mac_address"].as_string().c_str();
                        std::string internal_robot_name = data["internal_robot_name"].as_string().c_str();

                        // Dodanie do listy robotów
                        _robots[mac_address] = Robot(mac_address, internal_robot_name);
                        std::cout << "Robot added: " << internal_robot_name << " (" << mac_address << ")" << std::endl;
                    } else {
                        std::cerr << "Invalid response format from robot." << std::endl;
                    }
                } else {
                    std::cerr << "Error receiving response: " << ec.message() << std::endl;
                }
            });

        // Oczekiwanie na zakończenie odpowiedzi lub timeout
        timer.async_wait([&](const beast::error_code &ec) {
            if (!ec && !response_received) {
                // Timer wygasł, brak odpowiedzi
                robot_websocket.close(websocket::close_code::normal); // Rozłączenie
                std::cerr << "No response from robot within 5 seconds. Disconnected." << std::endl;
            }
        });

        // Wymagana asynchroniczna pętla do oczekiwania na operacje
        robot_websocket.get_executor().context().run_for(std::chrono::seconds(5));
    } catch (const std::exception &e) {
        std::cerr << "Exception in RobotManager::onNewRobotConnection: " << e.what() << std::endl;
    }
}
