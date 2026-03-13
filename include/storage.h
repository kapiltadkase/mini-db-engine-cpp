#ifndef STORAGE_H
#define STORAGE_H

#include <string>
#include "../include/record.h"
#include <unordered_map>
#include <vector>

class Storage{
    private:
    std::string filename;

    public:

    Storage(std::string file);
    // Just Declaring the function
    void insertRecord(const Record& record);

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


};

#endif