#ifndef STORAGE_H
#define STORAGE_H

#include <string>
#include "../include/record.h"
#include <unordered_map>
#include <vector>



class Storage{
    private:
    std::string filename;
    std::string metaFile;
    

    public:

    int nextId;
    std::vector<std::string> columns;

    Storage(std::string file);
    // Just Declaring the function
    void insertRecord(const std::string& name , int age);

    Record readRecord(int index);

    void printAllRecords();

    int getActiveRecordCount();

    int getRecordCount();

    void updateRecord(int index, const Record& record);
    
    void deleteRecord(int index);

    void findByName(const std::string& name);

    // Implementing hash index
    std::unordered_map<std::string, std::vector<int>> nameIndex;
    void buildIndex();

    void loadMetaData();
    void saveMetaData();


};

#endif