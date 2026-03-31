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
    
    int getColumnIndex(const std::string& columnName);
    

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

    void findByColumn(const std::vector<std::pair<std::string,std::string>>& conditions);

    // Implementing hash index
    std::unordered_map<std::string , std::unordered_map<std::string,std::vector<int>>> index;

    void buildIndex();

    void loadMetaData();
    void saveMetaData();

    void printFormatted(const std::vector<std::vector<std::string>>& rows);

    bool validateConditions(const std::vector<std::pair<std::string,std::string>>& conditions);


};

#endif