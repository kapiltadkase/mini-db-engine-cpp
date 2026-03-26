#include "../include/storage.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>


Storage :: Storage(std::string file){
    filename = file;

    metaFile = file.substr(0,file.find(".db")) + ".meta";

    buildIndex();
    loadMetaData();
}

void Storage :: loadMetaData(){
    std::ifstream file(metaFile);

    if(!file){
        // If file doesnt exist calculate nextId and create Meta data
        nextId = getRecordCount() + 1;
        return;
    }

    std::string line;
    bool foundNextId = false;

    while(std::getline(file, line)){
        if(line.find("nextId=")==0){
            nextId = std::stoi(line.substr(7));
            foundNextId = true;
        }
        else if(line.find("columns=")==0){
            std::stringstream ss(line.substr(8));
            std::string col;

            while(ss >> col){
                columns.push_back(col);
            }

        }
    }
    
    file.close();

    // Safety fallback if corrupted file
    if(!foundNextId){
        nextId = getRecordCount() + 1; 
    }

}

void Storage :: saveMetaData(){

    if(metaFile.empty()){
        std::cout<<"Meta file path is empty! \n";
        return;
    }
    std::ofstream file(metaFile);

    if(!file){
        std::cout<<"Failed to write metadata\n";
        return;
    }

    file <<"nextId="<<nextId<<"\n";

    file<<"columns=";
    for(const auto& col : columns){
        file << col << " ";
    }

    file << "\n";

    file.close();
}

void Storage :: buildIndex(){
    // opening file for reading in binary mode
    std::ifstream file(filename, std::ios::binary);

    if(!file){
        return;
    }
    
    Record r;
    int index = 0;
    while(file.read(reinterpret_cast<char*> (&r),sizeof(Record))){
        if(r.isActive){
           nameIndex[r.name].push_back(index);
        }
        index++;
    }

    file.close();
}

void Storage :: insertRecord(const std::vector<std::string>& values){
   // Implementing Schema Validation
   // STEP 1 : Validate the number of values
   if(values.size() != columns.size()){
        std::cout<< "Schema mismatch! Expected "<<columns.size()<<" values but got "<<values.size()<<"\n";
        return;
   }
   Record r;
   r.id = nextId++;
   saveMetaData();

   r.isActive = true;

   strcpy(r.name , values[0].c_str());

   try{
    r.age = std::stoi(values[1]);
   }
   catch(...){
    std::cout<< "Invalid age format\n";
    return;
   }
   
   // Text Based Storage
   std::ofstream outFile(filename,std::ios::app);

   if(!outFile){
        std::cout<<"1.Failed to open the file\n";
   }

   //Format : id|isActive| values....
   std::string row = std::to_string(r.id) + "|1";
    
   for(const auto& val : values){
        row += "|" + val;
   }

   outFile << row <<"\n";
   outFile.close();

   std::cout<<"Record Inserted\n";
}

void Storage :: printAllRecords(){
    std::ifstream inFile(filename);

    if(!inFile){
        std::cout<< "Failed to open file\n";
        return;
    }

    std::string line;
    while(std::getline(inFile,line)){
        if(line.empty()){
            continue;
        }

        std::stringstream ss(line);

        std::string token;
        std::vector<std::string> row;

        while(std::getline(ss,token,'|')){
            row.push_back(token);
        }

        // row format: id|isActive|values....
        if(row.size()>1 && row[1]=="1"){
            std::cout<< line <<"\n";
        }
    }

    inFile.close();
    
}

std::vector<std::string> Storage ::  readRecord(int index){
    //Opening file in binary mode for reading
    std::ifstream inFile(filename, std::ios::binary);
    
    if (!inFile)
    {
        std::cout<<"3.Failed to open file for reading \n";
        return {};
    }
    
    std::string line;
    int currentIndex = 0;
    
    while(std::getline(inFile,line)){
        
        if(line.empty()){
            continue;
        }

        if(currentIndex == index){
            std::vector<std::string> row;
            std::stringstream ss(line);
            std::string token;
            
            while(std::getline(ss,token, '|')){
                row.push_back(token);
            }

            inFile.close();
            return row;
        }

        currentIndex++;
    }

    inFile.close();
    return {};
    
}

int Storage :: getRecordCount(){
    // Opening File in read mode (opening at the end)
    std::ifstream inFile(filename , std::ios::binary | std::ios::ate);
    
    if(!inFile){
        std::cout<<"4.Failed to open file\n";
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
        std::cout<<"6.Failed to open file\n";
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


void Storage :: updateRecord(int index, const std::vector<std::string>& newValues){
    // Read the existing record
    auto row = readRecord(index);

    if(row.empty()){
        std::cout<<"Invalid index\n";
        return;
    }

    if(row[1] == "0"){
        std::cout<< "Cannot update deleted record\n";
        return;
    }

    if(newValues.size() != columns.size()){
        std::cout<< "Invalid number of values\n";
        return;
    }

    // Updating row (skiping the id and isActive)
    for(int i=0;i<newValues.size();i++){
        row[i+2] = newValues[i];  // row[0] = id and row[1] = isActive
    }

    // Rewriting the file
    std::ifstream inFile(filename);
    std::ofstream tempFile("temp.db");

    std::string line;
    int currentIndex = 0;

    while(std::getline(inFile,line)){
        
        if(currentIndex == index){
            // writing updated row
            for(int i =0;i<row.size();i++){
                tempFile << row[i];
                
                if(i != row.size()-1){
                    tempFile << "|";
                }
            }

            tempFile << "\n";
        }
        else{
            // if we are not on the required index push the original lines into the file
            tempFile << line << "\n";
        }

        currentIndex++;
    }

    inFile.close();
    tempFile.close();

    // Replacing the original file 
    remove(filename.c_str());
    rename("temp.db", filename.c_str());

    std::cout<<"Record Updated\n";

}

void Storage :: deleteRecord(int index){
    std::ifstream inFile(filename);
    if(!inFile){
        std::cout<<"Failed to open file\n";
        return;
    }

    std::vector<std::string> lines;
    std::string line;
    int currentIndex = 0;
    bool found = false;

    while(std::getline(inFile,line)){
        if(line.empty()){
            continue;
        }

        if(currentIndex == index){
            std::stringstream ss(line);
            std::string token;
            std::vector<std::string> row;

            while(std::getline(ss,token, '|')){
                row.push_back(token);
            }

            // row format: id|isActive|values....
            if(row.size() > 1){
                row[1] = "0";  // mark as deleted;
            }

            // rebuilding line
            std::string newLine = row[0];
            for(int i=1;i<row.size(); i++){
                newLine += "|" + row[i];
            }

            lines.push_back(newLine);
            found = true;
        }
        else{
            lines.push_back(line);
        }

        currentIndex++;
    }

    inFile.close();

    if(!found){
        std::cout<< "Invalid index\n";
        return;
    }

    // rewrite the file
    std::ofstream outFile(filename,std::ios::trunc);

    for(const auto& l : lines){
        outFile<< l <<"\n";
    }

    outFile.close();

    std::cout<< "Record deleted\n";
}

// Scanning file line by line , match the name and check if its isActive and then print
void Storage :: findByColumn(const std::string& columnName, const std::string& value){
    
    std::ifstream inFile(filename);
    if(!inFile){
        std::cout<<"Failed to open file\n";
        return;
    }

    // Find column index dynamically

    int colIndex = -1;
    for(int i=0;i<columns.size();i++){
        if(columns[i]==columnName){
            colIndex = i ; // offset for id and isActive
            break;
        }

    }

    if(colIndex == -1){
        std::cout<<"Column not found\n";
        return;
    }

    std::string line;
    bool found = false;

    while(std::getline(inFile,line)){
        if(line.empty()){
            continue;
        }

        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> row;

        while(std::getline(ss, token, '|')){
            row.push_back(token);
        }

        if(row.size() < columns.size() + 2 ){
            continue;
        }

        if(row[1] == "0"){
            continue;
        }
        
        // offset (id + isActive)
        if(row[colIndex + 2] == value){
            for(auto &col : row){
                std::cout<< col << " ";
            }

            std::cout<<"\n";

            found = true;
        }
    }

    inFile.close();

    if(!found){
        std::cout<<"No record found\n";
    }
}