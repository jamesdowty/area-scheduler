/******************************************
 * Program:      Area Scheduler           *
 * File:         event.h                  *
 * Author:       James Dowty              *
 * Last Updated: 07/14/2021               *
 * -------------------------------------- *
 * The event class holds the data for an  *
 * event.  An event is when an employee   *
 * status change occurs.  Ex: Employee    *
 * must be removed from area for break.   *
 * ****************************************/

#ifndef EVENT_H
#define EVENT_H

#include "employee.h"
#include "eventCatagory.h"
#include <string>

using namespace std;

class event
{
public:
    event(eventCatagory, employee*, short, short, int);
    short getMin() const;
    short getMax();
    employee* getEmp();
    bool operator>(const event &a) const {return min > a.getMin();};
private:
    eventCatagory catagory;  //The type of event
    int breakIndex;          //The name of the event (if event=REST)
    employee *emp = NULL; //Pointer to the employee
    short min;            //Minimum point in time event can be executed (in minutes)
    short max;            //Maximum point in time the event must be executed (in minutes)
};


event::event(eventCatagory e, employee *em, short mi, short ma, int n = -1)
{
    catagory = e;
    breakIndex = n;
    emp = em;
    min = mi;
    max = ma;
}

short event::getMin() const
{
    return min;
}

short event::getMax()
{
    return max;
}

employee* event::getEmp()
{
    return emp;
}

#endif