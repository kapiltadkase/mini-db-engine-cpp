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

Record Storage ::  readRecord(int index){
    //Opening file in binary mode for reading
    std::ifstream inFile(filename, std::ios::binary);
    
    if (!inFile)
    {
        std::cout<<"3.Failed to open file for reading \n";
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


void Storage :: updateRecord(int index, const Record& record){
    if(index < 0 || index >= getRecordCount()){
        std::cout << "Invalid index\n";
        return;
    }
    // Opening File in read mode and write mode (binary)
    std:: fstream File(filename,std::ios::binary | std::ios::in | std::ios::out);

    if (!File)
    {
        std::cout<<"7.Failed to open File\n";
        return;
    }
    
    auto offset = index * sizeof(Record);
    
    File.seekp(offset,std::ios::beg);

    File.write(reinterpret_cast<const char*>(&record),sizeof(Record));

    File.close();
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

void Storage :: findByName(const std::string& name){
    
    if(nameIndex.find(name) == nameIndex.end()){
        std::cout<<"No record found\n";
        return;
    }

    std::vector<int> positions = nameIndex[name];

    for(int pos : positions){
        Record r = readRecord(pos);

        if(r.isActive){
            std::cout<<r.id<<" "<<r.name<<" "<<r.age<<std::endl;
        }
    }

}