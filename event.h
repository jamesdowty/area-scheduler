#ifndef EVENT_H
#define EVENT_H

#include <string>

struct event
{
    eventCatagory event;
    string name;
    employee *emp = NULL;
    short min;
    short max;
};

#endif