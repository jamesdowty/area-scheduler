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
};

void department::importData(string areasPath, string breaksPath, string employeesPath)
{
    ifstream areasFile;
    areasFile.open(areasPath, ios::in);
    while(!areasFile.eof())
    {
        string title;
        int min, max;
        getline(areasFile, title);
        areasFile >> min >> max;

        areas.push_back(area(title, min, max));
    }

    areasFile.close();

    for(auto x : areas)
    {
        cout << "Name: " << x.getName();
        cout << "\nMin Employees: " << x.getMinEmployees();
        cout << "\nMax Employees: " << x.getMaxEmployees();
        cout << endl << endl;
    }

    return;
};

#endif