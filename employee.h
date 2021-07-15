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
    short getNextRest() {return nextRest;};
    eventCatagory getStatus(){return status;};
    area* getCurrentArea() {return currentArea;};
    void setCurrentArea(area *n) {currentArea = n;};
    void setStatus(eventCatagory x){status = x;};
    void incrementBreak(){nextRest++;};

private:
    string name;
    string id;
    short start;
    short end;
    short nextRest;
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
    nextRest = 0;
};

#endif