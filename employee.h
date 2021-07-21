#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>
#include <iostream>
#include "area.h"
#include "eventCatagory.h"

using namespace std;

class employee
{
public:
    employee() {};
    employee(string, string, int, int);
    string getName();
    string getId();
    short getStart();
    short getEnd();
    short getNextRest();
    eventCatagory getStatus();
    area* getCurrentArea();
    void setCurrentArea(area *n);
    void setStatus(eventCatagory x);
    void incrementBreak();

private:
    string name;
    string id;
    short start;
    short end;
    short nextRest;
    eventCatagory status;
    area *currentArea;
};

#endif