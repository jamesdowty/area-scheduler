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
    step(int, STEP_TYPE, string, int, int, int, int);
    step(int, STEP_TYPE, string, int, int, int);
    step(int, STEP_TYPE, string, int, int);
    int getTime() const;
    void printMessage();
    bool operator<(const step &x);
private:
    int time;
    STEP_TYPE stepType;
    int       employeeIn;
    int       employeeOut;
    int       where;
    int       whereFrom;
    string    message;
};

#endif