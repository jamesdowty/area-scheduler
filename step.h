#ifndef STEP_H
#define STEP_H

#include <string>
#include <iostream>
#include "employee.h"
#include "area.h"
#include "stepType.h"

using namespace std;

class step
{
public:
    step(int, STEP_TYPE, string, employee*, area*, employee*, area*);
    step(int, STEP_TYPE, string, employee*, area*, employee*);
    step(int, STEP_TYPE, string, employee*, area*);
    int getTime() const;
    void printMessage();
    bool operator<(const step &x);
private:
    int time;
    STEP_TYPE stepType;
    employee *employeeIn;
    employee *employeeOut;
    area     *where;
    area     *whereFrom;
    string   message;
};

#endif