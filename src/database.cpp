#include "../include/database.h"
#include <fstream>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

Database :: Database(){
    loadTables();
}

void Database :: loadTables(){
    std::string folder = "tests";

    if(!fs::exists(folder)){
        std::cout<< "No tests folder found\n";
        return;
    }

    for(const auto& entry : fs::directory_iterator(folder)){
        if(!entry.is_regular_file()){
            continue;
        }

        std::string filename = entry.path().filename().string();

        if(filename.size() > 3 && filename.substr(filename.size() - 3)== ".db"){
            std::string tableName = filename.substr(0,filename.size()-3);

            std::string fullPath = folder + "/" + filename;

            tables[tableName] = new Storage(fullPath);

            std::cout<<"Loaded table: "<<tableName<<"\n";
        }
    }

}


void Database :: createTable(const std::string& name, const std::vector<std::string>& columns){
    
    // If table already exists let the user know
    if(tables.find(name) != tables.end()){
        std::cout<<"Table already exists\n";
        return;
    }

    std::string filename = "tests/" + name + ".db";

    std::ofstream file(filename, std::ios::binary);

    if(!file){
        std::cout<<"Failed to create table\n";
        return;
    }

    file.close();

    // Create Storage obj
    Storage* table = new Storage(filename);

    // Set Schemna
    table->columns = columns;
    
    table->nextId = 1;   // Initialize nexId


    tables[name] = table;  // Store in map

    tables[name]->saveMetaData();

    

    std::cout<<"Table "<<name<<" created\n";

}

Storage* Database :: getTable(const std::string& tableName){

    if(tables.empty() || tables.find(tableName) == tables.end()){
        std::cout<<"No table found\n";
        return NULL;
    }

    return tables[tableName];

}

void Database :: showTables(){
    
    if(tables.empty()){
        std::cout<<"No tables found\n";
        return;
    }
    
    for(auto &table : tables){
        std::cout<<table.first<<std::endl;
    }

}