#ifndef DATABASE_H
#define DATABASE_H

#include <unordered_map>
#include <string>
#include "storage.h"

class Database{
    private:
        std::unordered_map<std::string, Storage*> tables;

    public:
        
        void loadTables();

        void createTable(const std::string& name);
        
        Storage* getTable(const std::string& name);
        
        void showTables();
    
        
};

#endif