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
      std::cout<<"\n";    // better spacing after command
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
      else if(command == "describe"){
         std::string tableName;
         ss >> tableName;

         Storage* table = db.getTable(tableName);

         if(!table){
            std::cout<<"Table not found\n";
            continue;
         }

         table->describeTable(tableName);
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

         std::string tableName;
         ss >> tableName;


         Storage* table = db.getTable(tableName);

         if(!table){
            std::cout<<"Tables not found\n";
            continue;
         }

         std::vector<std::string> values;
         std::string value;

         while(ss >> value){
            values.push_back(value);
         }
         
         int nextId = table->getRecordCount().first +1;
         
         table->insertRecord(values);
   
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

         auto r = table->readRecord(index-1);

         if(r[1] == "1"){
            for(int i=1;i<r.size();i++){
               std::cout<<r[i]<<" ";
            }
            std::cout<<"\n";
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
         
      }
      else if(command == "count"){
         std::string tableName;
         ss>> tableName;

         Storage * table = db.getTable(tableName);

         if(!table){
            std::cout<<"Table not found\n";
            continue;
         }

         int total = table->getRecordCount().first;
         int active = table->getRecordCount().second;

         std::cout<<"Total records: "<<total<<"\n"<<"Active records: "<<active<<"\n";
      }
      else if(command == "update"){
        std::string tableName;
        int index;

        ss >> tableName >> index;

        Storage*  table = db.getTable(tableName);

        if(!table){
            continue;
        }

        std::vector<std::string> values;
        std::string val;

        while(ss >>  val){
            values.push_back(val);
        }

        table->updateRecord(index-1,values);



      }
      else if(command == "find"){
         std::string tableName;
         ss >> tableName;
         Storage* table = db.getTable(tableName);

         if(!table){
            std::cout<<"Table not found\n";
            continue;
         }
         
         std::vector<std::pair<std::string,std::string>> conditions ;
         
         std::string word;
         int limit = -1;

         while(ss >> word){
            if(word == "limit"){
               ss >> limit;
            }
            else{
               std::string value;
               ss >> value;
               conditions.push_back({word,value});
            }
         }

         if(conditions.empty()){
            std::cout<<"Error: No condition provided\n";
            continue;
         }

         if(!table->validateConditions(conditions)){
            continue;
         }

         table->findByColumn(conditions,limit);

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

      std::cout<<"\n";  // spacing before next command input
      

   }
    

   return 0;
}