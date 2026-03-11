#include <iostream>
#include<sstream>
#include "../include/record.h"
#include "../include/storage.h"



int main(){
    
   Storage storage("tests/data.db");
   
   // Reading User input in a loop
   std::string input;
   while(true){
      std::cout<<"MiniDB > ";
      std::getline(std::cin,input);

      if(input=="exit"){
         break;
      }

      std::cout<<"You Entered: "<<input<<std::endl;
   }
    

   return 0;
}