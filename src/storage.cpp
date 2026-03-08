#include "../include/storage.h"
#include <fstream>
#include <iostream>


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
        std::cout<<r.id<<" "<<r.name<<" "<<r.age<<" "<<r.isActive<<std::endl;
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
    int offset = index * sizeof(Record);

    inFile.seekg(offset,std::ios::beg); // moves the file pointer
    // Now the pointer to the file is pointing to the record with at given index using the offset method
    inFile.read(reinterpret_cast<char*>(&r),sizeof(Record));  // Storing the value of record at given index in the memory of Record r
    
    inFile.close();

    return r;
}