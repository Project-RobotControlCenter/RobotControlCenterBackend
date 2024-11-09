//
// Created by elejandc on 11/9/24.
//

#ifndef DATABASEDAO_H
#define DATABASEDAO_H
#include "DatabaseManager.h"

#define DATABASE_NAME "RobocControlCenter"

class DatabaseDAO {
public:
    DatabaseDAO(const DatabaseDAO&) = delete;
    DatabaseDAO& operator=(const DatabaseDAO&) = delete;

    static DatabaseDAO& getInstance() {
        static DatabaseDAO _instance;
        return _instance;
    }

    bool insertDocument(const std::string& collection_name, const bsoncxx::document::view& document);
    bool deleteDocument(const std::string& collection_name, const bsoncxx::document::view& filter);
    bsoncxx::stdx::optional<bsoncxx::document::value> findDocument(const std::string& collection_name, const bsoncxx::document::view& filter);

private:
    DatabaseDAO();
    ~DatabaseDAO();

    DatabaseManager * database_manager = DatabaseManager::getInstance();
};



#endif //DATABASEDAO_H
