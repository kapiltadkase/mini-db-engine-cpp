#include <bits/stdc++.h>
#include "../include/record.h"

using namespace std;


int main(){
    
   ofstream file("tests/data.db", ios::binary | ios::app);
   
   if (!file) {
    std::cout << "File failed to open\n";
   }

   Record r1;
   r1.id = 1;
   strcpy(r1.name, "Kapil");
   r1.age = 23;
   r1.isActive = true;
   r1.isDeleted = false;

   Record r2;
   r2.id = 2;
   strcpy(r2.name,"Varad");
   r2.age = 21;
   r2.isActive = true;
   r2.isDeleted = false;

   Record r3;
   r3.id  = 3;
   strcpy(r3.name , "Gautam");
   r3.age = 21;
   r3.isActive = true;
   r3.isDeleted = false;
   

   file.write(reinterpret_cast<char*>(&r1), sizeof(Record));
   file.write(reinterpret_cast<char*>(&r2), sizeof(Record));
   file.write(reinterpret_cast<char*>(&r3), sizeof(Record));
   file.close();
    

    return 0;
}