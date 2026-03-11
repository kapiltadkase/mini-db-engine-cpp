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

int Storage :: getRecordCount(){
    // Opening File in read mode (opening at the end)
    std::ifstream inFile(filename , std::ios::binary | std::ios::ate);
    
    if(!inFile){
        std::cout<<"Failed to open file\n";
        return 0;
    }

    int FileSize = inFile.tellg();  //
    
    inFile.close();


    return FileSize/sizeof(Record);

}


void Storage :: updateRecord(int index, const Record& record){
    // Opening File in read mode (binary)
    std:: fstream File(filename,std::ios::binary | std::ios::in | std::ios::out);

    if (!File)
    {
        std::cout<<"Failed to open File\n";
        return;
    }
    
    int offset = index * sizeof(Record);
    
    File.seekg(offset,std::ios::beg);

    File.write(reinterpret_cast<const char*>(&record),sizeof(Record));

    File.close();
}