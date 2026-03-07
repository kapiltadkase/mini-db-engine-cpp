#ifndef RECORD_H
#define RECORD_H

struct Record{
    int id;
    char name[50];
    int age;
    bool isActive;
    bool isDeleted;
};

#endif