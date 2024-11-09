//
// Created by elejandc on 11/9/24.
//

#include "DatabaseDAO.h"

#include <iostream>



DatabaseDAO::DatabaseDAO() {
}

DatabaseDAO::~DatabaseDAO() {
}


bool DatabaseDAO::insertDocument(const std::string &collection_name, const bsoncxx::document::view &document) {
    try {
        mongocxx::client* client = DatabaseManager::getConnection().get();
        if (client) {
            auto collection = (*client)[DATABASE_NAME][collection_name];
            collection.insert_one(document);
            return true;
        }
    } catch (const std::exception& e) {
        std::cerr << "ERROR: Failed to insert document: " << e.what() << std::endl;
    }
    return false;
}

bool DatabaseDAO::deleteDocument(const std::string &collection_name, const bsoncxx::document::view &filter) {
    try {
        mongocxx::client* client = DatabaseManager::getConnection().get();
        if (client) {
            auto collection = (*client)[DATABASE_NAME][collection_name];
            auto result = collection.delete_one(filter);
            return result && result->deleted_count() > 0;
        }
    } catch (const std::exception& e) {
        std::cerr << "ERROR: Failed to delete document: " << e.what() << std::endl;
    }
    return false;
}
