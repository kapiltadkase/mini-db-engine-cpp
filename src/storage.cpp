#include "../include/storage.h"
#include <fstream>
#include <iostream>
#include <cstring>


Storage :: Storage(std::string file){
    filename = file;
}

void Storage :: insertRecord(const Record& record){
    // opening file in binary mode for writing
    std::ofstream outFile(filename,std::ios::binary | std::ios::app);
    if (!outFile)
    {
        std::cout<<"Failed to open the file for writing. \n";
        return;
    }
    
    outFile.write(reinterpret_cast<const char*>(&record),sizeof(Record));

    outFile.close();
    
}

void Storage :: printAllRecords(){
    //Opening file in binary mode for reading
    std::ifstream inFile(filename,std::ios::binary);

    if (!inFile)
    {
        std::cout<<"Failed to open the file for reading. \n";
        return;
    }

    Record r;
    
    while(inFile.read(reinterpret_cast<char*>(&r),sizeof(Record))){
        if(r.isActive == true){
           std::cout<<r.id<<" "<<r.name<<" "<<r.age<<" "<<r.isActive<<std::endl; 
        }
    }

    inFile.close();
    
}

Record Storage ::  readRecord(int index){
    //Opening file in binary mode for reading
    std::ifstream inFile(filename, std::ios::binary);
    
    if (!inFile)
    {
        std::cout<<"Failed to open file for reading \n";
        return Record();
    }

    Record r;
    
    // Fetching record using offset calculation
    auto offset = index * sizeof(Record);

    inFile.seekg(offset,std::ios::beg); // moves the file pointer
    // Now the pointer to the file is pointing to the record with at given index using the offset method
    inFile.read(reinterpret_cast<char*>(&r),sizeof(Record));  // Storing the value of record at given index in the memory of Record r
    
    inFile.close();

    return r;
}

int Storage :: getRecordCount(){
    // Opening File in read mode (opening at the end)
    std::ifstream inFile(filename , std::ios::binary | std::ios::ate);
    
    if(!inFile){
        std::cout<<"Failed to open file\n";
        return 0;
    }

    auto fileSize = inFile.tellg();  //
    
    inFile.close();


    return fileSize/sizeof(Record);

}

int Storage :: getActiveRecordCount(){
     // Opening File in read mode
    std::ifstream inFile(filename , std::ios::binary);
    
    if(!inFile){
        std::cout<<"Failed to open file\n";
        return 0;
    }

    Record r;
    int count = 0;
    while(inFile.read(reinterpret_cast<char*> (&r),sizeof(Record))){
        if(r.isActive){
            count++;
        }
    }
    
    inFile.close();

    return count;
}


void Storage :: updateRecord(int index, const Record& record){
    if(index < 0 || index >= getRecordCount()){
        std::cout << "Invalid index\n";
        return;
    }
    // Opening File in read mode and write mode (binary)
    std:: fstream File(filename,std::ios::binary | std::ios::in | std::ios::out);

    if (!File)
    {
        std::cout<<"Failed to open File\n";
        return;
    }
    
    auto offset = index * sizeof(Record);
    
    File.seekp(offset,std::ios::beg);

    File.write(reinterpret_cast<const char*>(&record),sizeof(Record));

    File.close();
}

void Storage :: deleteRecord(int index){
    if(index < 0 || index >= getRecordCount()){
        std::cout << "Invalid index\n";
        return;
    }
    Record r = readRecord(index);
    r.isActive = false;

    updateRecord(index,r);
}

void Storage :: findByName(const std::string& name){
    // Opening file in read mode
    std::ifstream file(filename, std::ios::binary);

    if(!file){
        std::cout<<"Failed to open file\n";
        return;
    }

    Record r;

    bool found = false;
    while(file.read(reinterpret_cast<char*>(&r),sizeof(Record))){
        if(r.isActive && strcmp(r.name, name.c_str()) == 0){  // r.name is char[50] and name is string
            std::cout<<r.id<<" "<<r.name<<" "<<r.age<<std::endl;
            found = true;
        }
    }

    if(!found){
        std::cout<<"No record found\n";
    }

    file.close();
}