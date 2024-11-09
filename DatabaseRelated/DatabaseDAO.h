//
// Created by elejandc on 11/9/24.
//

#ifndef DATABASEDAO_H
#define DATABASEDAO_H
#include "DatabaseManager.h"


class DatabaseDAO {
public:
    DatabaseDAO(const DatabaseDAO&) = delete;
    DatabaseDAO& operator=(const DatabaseDAO&) = delete;

    static DatabaseDAO& getInstance() {
        static DatabaseDAO _instance;
        return _instance;
    }

private:
    DatabaseDAO();
    ~DatabaseDAO();

    DatabaseManager * database_manager = DatabaseManager::getInstance();
};



#endif //DATABASEDAO_H
