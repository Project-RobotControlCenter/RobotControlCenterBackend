//
// Created by elejandc on 11/8/24.
//

#include "DatabaseManager.h"

#include <iostream>
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/exception/exception.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/options/client.hpp>
#include <mongocxx/options/apm.hpp>

std::unique_ptr<DatabaseManager> DatabaseManager::_instance = nullptr;
std::unique_ptr<mongocxx::client> DatabaseManager::_client_local = nullptr;
std::unique_ptr<mongocxx::client> DatabaseManager::_client_remote = nullptr;

DatabaseManager::DatabaseManager(const char *local_db_ip, const char *local_db_port, const char *remote_db_ip,
    const char *remote_db_port, const char *db_password) {
    mongocxx::instance instance{};

    auto options = createClientOptions();

    // Connect to local database
    _client_local = connect(local_db_ip, local_db_port, db_password, options);
    if(_client_local) {
        std::cout << "INFO : DatabaseManager - CONNECTED TO LOCAL DATABASE" << std::endl;
    } else {
        std::cerr << "ERROR : DatabaseManager - FAILED TO CONNECT TO LOCAL DATABASE" << std::endl;
    }

    // Connect to remote database
    _client_remote = connect(remote_db_ip, remote_db_port, db_password, options);
    if(_client_remote) {
        std::cout << "INFO : DatabaseManager - CONNECTED TO REMOTE DATABASE" << std::endl;
    } else {
        std::cerr << "ERROR : DatabaseManager - FAILED TO CONNECT TO REMOTE DATABASE" << std::endl;
    }

    if(_client_local || _client_remote) {
        std::cout << "INFO : DatabaseManager - CONNECTED TO DATABASES" << std::endl;
    }
}

DatabaseManager::~DatabaseManager() {
    std::cout << "INFO : DatabaseManager - DESTRUCTOR" << std::endl;
}

std::unique_ptr<mongocxx::client> DatabaseManager::connect(const char *db_ip, const char *db_port, const char *db_password, const mongocxx::options::client &options) {
    try {
        const std::string url = "mongodb://admin:" + std::string(db_password) + "@" + std::string(db_ip) + ":" + std::string(db_port);
        return std::move(std::make_unique<mongocxx::client>(mongocxx::uri{url}, options));
    }catch (const std::exception& e) {
        std::cerr << "ERROR : Can't connect to database: " << e.what() << std::endl;
        return nullptr;
    };
}

bool DatabaseManager::isConnected(const std::unique_ptr<mongocxx::client> &client) {
    if (!client) {
        std::cerr << "Error: Client pointer is null." << std::endl;
        return false;
    }

    try {
        auto admin_db = client->database("admin");
        bsoncxx::builder::stream::document command;
        command << "ping" << 1;

        auto result = admin_db.run_command(command.view());
        auto ok_element = result.view()["ok"];

        if (ok_element && ok_element.type() == bsoncxx::type::k_double) {
            return ok_element.get_double() == 1.0;
        } else {
            std::cerr << "Error: Unexpected response type or missing 'ok' field." << std::endl;
            return false;
        }
    } catch (const mongocxx::exception& e) {
        std::cerr << "Error : MongoDB exception: " << e.what() << std::endl;
        return false;
    } catch (const std::exception& e) {
        std::cerr << "Error : Standard exception: " << e.what() << std::endl;
        return false;
    } catch (...) {
        std::cerr << "Error : Unknown error occurred while checking connection." << std::endl;
        return false;
    }
}

mongocxx::options::client DatabaseManager::createClientOptions() {
    mongocxx::options::apm apm_opts;
    apm_opts.on_heartbeat_started([](const mongocxx::events::heartbeat_started_event& event) {
        //std::cout << "Server heartbeat started for server: " << event.host().name << ":" << event.host().port << std::endl;
    });

    apm_opts.on_heartbeat_succeeded([](const mongocxx::events::heartbeat_succeeded_event& event) {
        //std::cout << "Server heartbeat succeeded for server: " << event.host().name << ":" << event.host().port << std::endl;
    });

    apm_opts.on_heartbeat_failed([](const mongocxx::events::heartbeat_failed_event& event) {
        //std::cerr << "Server heartbeat failed for server: " << event.host().name << ":" << event.host().port
        //          << " with error: " << event.failure().message() << std::endl;
    });

    mongocxx::options::client client_options;
    client_options.apm_opts(apm_opts); // Ustawienie opcji APM dla klienta

    return client_options;
}

