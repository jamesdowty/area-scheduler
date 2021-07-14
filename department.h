#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include "employee.h"
#include "area.h"
#include "rest.h"
#include <unordered_map>
#include <queue>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

class department
{
public:
    void importData(string, string, string);
    
private:
    vector<employee> employees;
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

int department::toMinutes(string in)
{
    int result;
    stringstream s(in);
    string hours;
    string minutes;
    getline(s, hours, ':');
    cout << hours;
    getline(s, minutes);
    result = stoi(hours) * 60;
    result += stoi(minutes);
    return result;
};

void department::importData(string areasPath, string breaksPath, string employeesPath)
{
    string title, startTime, endTime, maxConcecutive, maxTotal, duration, min, max;
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

        breaks.push_back(rest(title, toMinutes(duration), toMinutes(min), toMinutes(max), paid == 'y' || paid == 'Y'));
    }

    inputFile.close();

    //EMPLOYEES INPUT FILE
    inputFile.open(employeesPath);

    while(!inputFile.eof())
    {
        string n, id, s, e; //name, employee id, start time, end time
        getline(inputFile, n);
        getline(inputFile, id);
        getline(inputFile, s);
        getline(inputFile, e);

        employees.push_back(employee(n, id, toMinutes(s), toMinutes(e)));
    }
    inputFile.close();

    /****************************************
     * PRINTING RESULTS FOR TEST
     * *************************************/
    cout << "__________AREAS__________\n";

    for(auto x : areas)
    {
        cout << "Name: " << x.getName() << endl;
        cout << "Min:  " << x.getMinEmployees() << endl;
        cout << "Max:  " << x.getMaxEmployees() << endl;
        cout << "Curr: " << x.getCurEmployees() << endl << endl;
    }

    cout << "\n__________BREAKS__________\n";

    for(auto x : breaks)
    {
        cout << "Name: " << x.getName() << endl;
        cout << "Duration: " << x.getDuration() << endl;
        cout << "Min Start: " << x.getMinStart() << endl;
        cout << "Max Start: " << x.getMaxStart() << endl;
    }

    cout << "\n__________EMPLOYEES__________\n";

    for(auto x : employees)
    {
        cout << "Name: " << x.getName() << endl;
        cout << "ID: " << x.getId() << endl;
        cout << "Start: " << x.getStart() << endl;
        cout << "End: " << x.getEnd() << endl;
    }

    return;
};

#endif