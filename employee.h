#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>
#include "area.h"

using namespace std;

struct employee
{
    string name;
    string id;
    short start;
    short end;
    //eventCatagory status;
    area currentArea;
};

#endif