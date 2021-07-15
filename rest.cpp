/******************************************
 * Program:      Area Scheduler           *
 * File:         rest.cpp                 *
 * Author:       James Dowty              *
 * Last Updated: 07/14/2021               *
 * -------------------------------------- *
 * Functing defenitions for rest class    *
 * ****************************************/

#include "rest.h"

rest::rest(std::string n, int dur, int min, int max, bool p)
{
    name = n;
    duration = dur;
    minStart = min;
    maxStart = max;
    paid = p;
};

std::string rest::getName() 
{
    return name;
};

int rest::getDuration()
{
    return duration;
};

int rest::getMinStart()
{
    return minStart;
};

int rest::getMaxStart()
{
    return maxStart;
};