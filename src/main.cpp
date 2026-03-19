#include <iostream>
#include <cstring> 
#include <sstream>
#include <fstream> 
#include "../include/record.h"
#include "../include/storage.h"
#include "../include/database.h"



int main(){
    

   Database db;
   
   // Reading User input in a loop (CLI)
   std::string input;


   while(true){
      std::cout<<"MiniDB > ";
      std::getline(std::cin,input);

      if(input=="exit"){
         break;
      }

      /*--------COMMAND PARSING ---------------*/ 
      
      // 1. Tokenzing the input
      std::stringstream ss(input);
      std::string command;
      
      // 2. Identification of Command (list, insert, read, delete, update, activeCount, totalCount, find, help)
      ss >> command;     // Extracted first token

      if(command == "createTable"){
         std:: string tableName;
         ss>> tableName;

         std::vector<std::string> columns;
         std::string col;

         while(ss >> col){
            columns.push_back(col);
         }

         if(tableName.empty()){
            std::cout<<"Usage: createTable <table_name> <col1> <col2> ....\n";
            continue;
         }
         
         std:: ofstream file("tests/" + tableName + ".db" , std::ios::binary);

         if(!file){
            std::cout<<"Failed to create table\n";
         }
         

         file.close();

         db.createTable(tableName, columns);

      }
      else if(command == "showTables"){
         db.showTables();
      }
      else if(command=="list"){
         std::string tableName;
         ss >> tableName;

         Storage* table = db.getTable(tableName);

         if(!table){
            std::cout<<"Table not found\n";
            continue;
         }

         table->printAllRecords();

      }
      else if(command=="insert"){

         std::string tableName ,name;
         int age;

         // Extracting the rest of the token
         if(!(ss >>tableName >> name >> age)){
            std::cout<<"Usage insert <name> <age>\n";
         }

         Storage* table = db.getTable(tableName);

         if(!table){
            std::cout<<"Tables not found\n";
            continue;
         }
         
         int nextId = table->getRecordCount() +1;
         
         table->insertRecord(name,age);
         std::cout<<"Record Inserted\n";
      }
      else if(command == "read"){
         std::string tableName;
         int index;
         ss >>tableName >> index;

         Storage* table = db.getTable(tableName);

         if(!table || index <= 0){
            std::cout << "Invalid index\n";
            continue;
         }

         Record r = table->readRecord(index-1);

         if(r.isActive == true){
            std::cout<<r.id<<" "<<r.name<<" "<<r.age<<std::endl;
         }
         else{
            std::cout<<"Record is deleted or inactive\n";
         }
      }
      else if(command == "delete"){
         std::string tableName;
         int index;
         
         if(!(ss>> tableName >> index)){
            std::cout<<"Usage: delete <table_name> <index>\n";
            continue;
         }

         Storage* table = db.getTable(tableName);

         if(!table || index <= 0){
            std::cout << "Invalid index\n";
            continue;
         }

         table->deleteRecord(index-1);   // 1-based indexing
         
         std::cout<<"Record deleted\n";
      }
      else if(command == "activeCount"){
         std::string tableName;
         ss>> tableName;

         Storage * table = db.getTable(tableName);

         if(!table){
            std::cout<<"Table not found\n";
            continue;
         }

         int count = table->getActiveRecordCount();

         std::cout<<"Total Active Records: "<<count<<std::endl;
      }
      else if(command == "totalCount"){

         std::string tableName;
         ss>> tableName;
         
         Storage* table = db.getTable(tableName);

         if(!table){
            std::cout<<"Table not found\n";
            continue;
         }


         int totalCount = table->getRecordCount();
         
         std::cout<<"Total Record Count: "<<totalCount<<std::endl;
      }
      else if(command == "update"){
         int index;
         std::string tableName,name;
         int age;
         
         ss >>tableName >> index >> name >> age;

         Storage* table = db.getTable(tableName);

         if(!table || index<=0){
            std::cout<<"Invalid index\n";
            continue;
         }
         
        Record r;
        r.id = index;
        r.age = age;
        r.isActive = true;
        strcpy(r.name,name.c_str());
        
        table->updateRecord(index-1,r);

         std::cout<<"Record Updated\n";

      }
      else if(command == "find"){
         std::string tableName, name;
         if(!(ss>>tableName >> name)){
            std::cout<<"Usage: find <name>\n";
            continue;
         }

         Storage* table = db.getTable(tableName);

         if(!table){
            std::cout<<"Table not found\n";
            continue;
         }
         
         table->findByName(name);

      }
      else if(command == "help"){
         std::cout << "Available commands:\n";
         std::cout << "insert <name> <age>\n";
         std::cout << "read <index>\n";
         std::cout << "update <index> <name> <age>\n";
         std::cout << "delete <index>\n";
         std::cout << "list\n";
         std::cout << "count\n";
         std::cout << "totalCount\n";
         std::cout<< "find <name>\n";
         std::cout << "exit\n";
         
      }
      else{
         std::cout<<"Unknown command. Type 'help' to see available commands.\n";
      }
      

   }
    

   return 0;
}