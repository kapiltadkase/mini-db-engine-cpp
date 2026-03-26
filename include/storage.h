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
    void insertRecord(const std::vector<std::string>& values);

    std::vector<std::string> readRecord(int index);

    void printAllRecords();

    int getActiveRecordCount();

    int getRecordCount();

    void updateRecord(int index, const std::vector<std::string>& newValues);
    
    void deleteRecord(int index);

    void findByColumn(const std::string& columnName, const std::string& value);

    // Implementing hash index
    std::unordered_map<std::string, std::vector<int>> nameIndex;
    void buildIndex();

    void loadMetaData();
    void saveMetaData();


};

#endif