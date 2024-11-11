//
// Created by elejandc on 11/9/24.
//

#include "DatabaseDAO.h"

#include <iostream>



DatabaseDAO::DatabaseDAO() {
    std::cout << "DEBUG : DatabaseDAO - CONSTRUCTOR" << std::endl;
}

DatabaseDAO::~DatabaseDAO() {
    std::cout << "DEBUG : DatabaseDAO - DESTRUCTOR" << std::endl;
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

bsoncxx::stdx::optional<bsoncxx::document::value> DatabaseDAO::findDocument(const std::string &collection_name,
    const bsoncxx::document::view &filter) {
    try {
        mongocxx::client* client = DatabaseManager::getConnection().get();
        if (client) {
            auto collection = (*client)[DATABASE_NAME][collection_name];
            return collection.find_one(filter);
        }
    } catch (const std::exception& e) {
        std::cerr << "ERROR: Failed to find document: " << e.what() << std::endl;
    }
    return bsoncxx::stdx::nullopt;
}

bool DatabaseDAO::updateDocument(const std::string &collection_name, const bsoncxx::document::view &filter,
    const bsoncxx::document::view &update) {
    try {
        mongocxx::client* client = DatabaseManager::getConnection().get();
        if (client) {
            auto collection = (*client)[DATABASE_NAME][collection_name];
            auto result = collection.update_one(filter, update);
            return result && result->modified_count() > 0;
        }
    } catch (const std::exception& e) {
        std::cerr << "ERROR: Failed to update document: " << e.what() << std::endl;
    }
    return false;
}
