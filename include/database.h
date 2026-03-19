#ifndef DATABASE_H
#define DATABASE_H

#include <unordered_map>
#include <string>
#include "storage.h"

class Database{
    private:
        std::unordered_map<std::string, Storage*> tables;

    public:
        
        Database();

        void loadTables();

        void createTable(const std::string& name, const std::vector<std::string>& columns);
        
        Storage* getTable(const std::string& tableName);
        
        void showTables();
    
        
};

#endif