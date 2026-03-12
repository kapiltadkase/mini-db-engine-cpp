#include <iostream>
#include <cstring> 
#include <sstream>
#include "../include/record.h"
#include "../include/storage.h"



int main(){
    
   Storage storage("tests/data.db");
   
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
      
      // 2. Identification of Command (list, insert, read, delete, update, activeCount, totalCount)
      ss >> command;     // Extracted first token
      
      if(command=="list"){
         storage.printAllRecords();
      }
      else if(command=="insert"){
         int id;
         std::string name;
         int age;

         // Extracting the rest of the token
         if(!(ss >> id >> name >> age)){
            std::cout<<"Usage insert <id> <name> <age>\n";
         }


         Record r;
         r.id = id;
         r.age = age;
         r.isActive = true;

         strcpy(r.name, name.c_str());

         storage.insertRecord(r);
         std::cout<<"Record Inserted\n";
      }
      else if(command == "read"){
         int index;
         ss >> index;

         if(index <= 0){
            std::cout << "Invalid index\n";
            continue;
         }

         Record r = storage.readRecord(index-1);

         if(r.isActive == true){
            std::cout<<r.id<<" "<<r.name<<" "<<r.age<<std::endl;
         }
         else{
            std::cout<<"Record is deleted or inactive\n";
         }
      }
      else if(command == "delete"){
         int index;
         
         if(!(ss >> index)){
            std::cout<<"Usage: delete <index>\n";
            continue;
         }

         if(index <= 0){
            std::cout << "Invalid index\n";
            continue;
         }

         storage.deleteRecord(index-1);   // 1-based indexing
         
         std::cout<<"Record deleted\n";
      }
      else if(command == "activeCount"){

         int count = storage.getActiveRecordCount();

         std::cout<<"Total Active Records: "<<count<<std::endl;
      }
      else if(command == "totalCount"){

         int totalCount = storage.getRecordCount();
         
         std::cout<<"Total Record Count: "<<totalCount<<std::endl;
      }
      else if(command == "update"){
         int index;
         std::string name;
         int age;
         
         ss >> index >> name >> age;

         if(index<=0){
            std::cout<<"Invalid index\n";
            continue;
         }
         
         index--;   // To convert it into 0-based indexing

         Record r;
         r.id = index+1;
         r.age = age;
         strcpy(r.name, name.c_str());
         r.isActive = true;

         storage.updateRecord(index,r);

         std::cout<<"Record Updated\n";

      }
      else{
         std::cout<<"Unknown command. Type 'help' to see available commands.\n";
      }
      

   }
    

   return 0;
}