#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include "employee.h"
#include "area.h"
#include "event.h"
#include "rest.h"
#include <unordered_map>
#include <queue>
#include <fstream>
#include <iostream>

using namespace std;

class department
{
public:
    void importData(string, string, string);
    
private:
    unordered_map<string, employee> employees;
    vector<area> areas;
    queue<employee> availableEmployees;
    //priority_queue<event> startTimes;
    //priority_queue<event> endTimes;
    //priority_queue<event> violations;
    vector<rest> breaks;
    int maxConcecutiveTime;
    int maxTotalTime;
    int parkOpen;
    int parkClose;
    int toMinutes(string);
};

void department::importData(string areasPath, string breaksPath, string employeesPath)
{
    string title, startTime, endTime, maxConcecutive, maxTotal, duration;
    int min, max;
    char paid;
    
    ifstream inputFile;

    //AREAS INPUT FILE
    inputFile.open(areasPath);
    while(!inputFile.eof())
    {
        int min, max;
        getline(inputFile, title);
        inputFile >> min;
        inputFile >> max;
        inputFile.ignore(10000, '\n');
        areas.push_back(area(title, min, max));
    }
    inputFile.close();

    //BREAKS INPUT FILE
    inputFile.open(breaksPath);

    inputFile >> maxConcecutive;
    inputFile >> maxTotal;
    maxConcecutiveTime = toMinutes(maxConcecutive);
    maxTotalTime = toMinutes(maxTotal);

    while(!inputFile.eof())
    {
        getline(inputFile, title);
        getline(inputFile, duration);
        getline(inputFile, min);
        getline(inputFile, max);
        inputFile.ignore(10000, '\n');
        inputFile.get(paid);

        breaks.push_back(title, toMinutes(duration), toMinutes(min), toMinutes(max), paid == 'y' || paid == 'Y');
    }

    inputFile.close();

    //EMPLOYEES INPUT FILE
    inputFile.open(employeesPath);
/*
    while(!inputFile.eof())
    {

    }
*/

    inputFile.close();
    return;
};

#endif