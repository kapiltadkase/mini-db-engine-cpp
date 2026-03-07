#include <bits/stdc++.h>
#include "../include/record.h"

using namespace std;


int main(){
    
    Record r;
    r.id = 1;
    r.age = 22;
    r.isActive = true;
    r.isDeleted = false;

    cout<<"Size of Record is: "<<sizeof(Record)<<endl;
    cout<<"Record Struct working"<<endl;
    

    return 0;
}