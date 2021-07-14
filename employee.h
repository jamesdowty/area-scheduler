#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>
#include "area.h"
#include "eventCatagory.h"

using namespace std;

class employee
{
public:
    employee() {};
    employee(string, string, short, short, eventCatagory, area*);
    string getName() {return name;};
    string getId() {return id;};
    short getStart() {return start;};
    short getEnd() {return end;};
    eventCatagory getStatus(){return status;};
    area* getCurrentArea() {return currentArea;};

private:
    string name;
    string id;
    short start;
    short end;
    eventCatagory status;
    area *currentArea;
};

employee::employee(string n, string i, short s, short e, eventCatagory x = OFF, area *a = NULL)
{
    name = n;
    id = i;
    start = s;
    end = e;
    status = x;
    currentArea = a;
};

#endif