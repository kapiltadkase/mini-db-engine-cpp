#include "../include/database.h"
#include <fstream>
#include <iostream>


void Database :: createTable(const std::string& name){

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

    std::cout<<"add table to map\n";

    tables[name] = new Storage(filename);  // Easy access to tables

    std::cout<<"Table "<<name<<" created\n";

}

Storage* Database :: getTable(const std::string& name){

    if(tables.empty() || tables.find(name) == tables.end()){
        std::cout<<"No table found\n";
        return NULL;
    }

    return tables[name];

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