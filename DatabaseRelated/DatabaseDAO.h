//
// Created by elejandc on 11/9/24.
//

#ifndef DATABASEDAO_H
#define DATABASEDAO_H
#include "DatabaseManager.h"

// #define DATABASE_NAME "RobocControlCenter"

class DatabaseDAO {
public:
    DatabaseDAO(const DatabaseDAO&) = delete;
    DatabaseDAO& operator=(const DatabaseDAO&) = delete;
    ~DatabaseDAO();

    static bool initInstance(const std::string& database_name) {
        if(!_instance) {
            _instance = std::unique_ptr<DatabaseDAO>(new DatabaseDAO(database_name));
        }

        return _instance != nullptr;
    }

    static DatabaseDAO* getInstance() {
        return _instance.get();
    }

    static bool insertDocument(const std::string& collection_name, const bsoncxx::document::view& document) {return getInstance()->insertDocumentImp(collection_name, document);}
    static bool deleteDocument(const std::string& collection_name, const bsoncxx::document::view& filter)  {return getInstance()->deleteDocumentImp(collection_name, filter);}
    static bsoncxx::stdx::optional<bsoncxx::document::value> findDocument(const std::string& collection_name, const bsoncxx::document::view& filter)  {return getInstance()->findDocumentImp(collection_name, filter);}
    static bool updateDocument(const std::string& collection_name, const bsoncxx::document::view& filter, const bsoncxx::document::view& update) {return getInstance()->updateDocumentImp(collection_name, filter, update);}

private:
    static std::unique_ptr<DatabaseDAO> _instance;
    std::string _database_name;

    explicit DatabaseDAO(std::string database_name);

    bool insertDocumentImp(const std::string& collection_name, const bsoncxx::document::view& document);
    bool deleteDocumentImp(const std::string& collection_name, const bsoncxx::document::view& filter);
    bsoncxx::stdx::optional<bsoncxx::document::value> findDocumentImp(const std::string& collection_name, const bsoncxx::document::view& filter);
    bool updateDocumentImp(const std::string& collection_name, const bsoncxx::document::view& filter, const bsoncxx::document::view& update);
};



#endif //DATABASEDAO_H
