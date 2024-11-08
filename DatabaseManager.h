//
// Created by elejandc on 11/8/24.
//

#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <memory>
#include <mongocxx/client.hpp>

#define MAX_RECONNECT_ATTEMPTS 5

class DatabaseManager {
public:
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager() = default;
    DatabaseManager& operator=(const DatabaseManager&) = delete;
    ~DatabaseManager();

    static bool initInstance(const char * local_db_ip, const char * local_db_port,const char * remote_db_ip, const char * remote_db_port, const char *db_password) {
        if(!_instance) {
            _instance = std::unique_ptr<DatabaseManager>(new DatabaseManager(local_db_ip, local_db_port, remote_db_ip, remote_db_port, db_password));
        }

        return _instance != nullptr;
    }

    static DatabaseManager* getInstance() {
        return _instance.get();
    }

    static std::unique_ptr<mongocxx::client> getConnection();

private:
    static std::unique_ptr<DatabaseManager> _instance;
    static std::unique_ptr<mongocxx::client> _client_local;
    static std::unique_ptr<mongocxx::client> _client_remote;

    std::string _local_db_ip;
    std::string _local_db_port;
    std::string _remote_db_ip;
    std::string _remote_db_port;
    std::string _db_password;

    bool connect_to_local = false;
    bool connect_to_remote = false;

    mongocxx::options::client _options;

    DatabaseManager(const char * local_db_ip, const char * local_db_port, const char * remote_db_ip, const char * remote_db_port, const char * db_password);

    std::unique_ptr<mongocxx::client> connect(const char * db_ip, const char * db_port, const char * db_password,  const mongocxx::options::client &options);
    bool isConnected(const std::unique_ptr<mongocxx::client>& client);

    mongocxx::options::client createClientOptions();
};



#endif //DATABASEMANAGER_H
