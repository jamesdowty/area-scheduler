#ifndef EVENT_H
#define EVENT_H

#include "employee.h"
#include <string>

using namespace std;

enum eventCatagory {START, END, REST, GAME, OFF};

struct event
{
    eventCatagory event;
    string name;
    employee *emp = NULL;
    short min;
    short max;
};

#endif