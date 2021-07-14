#ifndef EVENT_H
#define EVENT_H

#include "employee.h"
#include "eventCatagory.h"
#include <string>

using namespace std;

struct event
{
    eventCatagory event;
    string name;
    employee *emp = NULL;
    short min;
    short max;
};

#endif