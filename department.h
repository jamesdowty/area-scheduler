#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include "employee.h"
#include "area.h"
#include "rest.h"
#include "event.h"
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

class department
{
public:
    void importData(string, string, string);
    void createSchedule();
    void printSteps();
    void printEmployeeSchedule();
    void printGameSchedule();
    
private:
    unordered_map<std::string, employee> employees;
    vector<area>    areas;
    queue<employee*> availableEmployees;
    priority_queue<event, vector<event>, greater<event>> startTimes;
    vector<event>   endTimes;
    vector<event>   violations;
    vector<rest>    breaks;
    vector<string>  steps;
    int maxConcecutiveTime;
    int maxTotalTime;
    int parkOpen;
    int parkClose;
    int currentTime;
    int toMinutes(string);
    void updateStartTimes();
    void handleViolations();
    void handleEndTimes();
    void fillGames();
    void consolidateGames();
};

int department::toMinutes(string in)
{
    int result;
    stringstream s(in);
    string hours;
    string minutes;
    getline(s, hours, ':');
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
    inputFile.ignore(10000, '\n');
    maxConcecutiveTime = toMinutes(maxConcecutive);
    maxTotalTime = toMinutes(maxTotal);

    while(!inputFile.eof())
    {
        getline(inputFile, title);
        getline(inputFile, duration);
        getline(inputFile, min);
        getline(inputFile, max);
        inputFile.get(paid);
        inputFile.ignore(10000, '\n');
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

        employees[id] = employee(n, id, toMinutes(s), toMinutes(e));
        startTimes.push(event(START, &employees[id], toMinutes(s), toMinutes(s)));
        endTimes.push_back(event(END, &employees[id], toMinutes(e), toMinutes(e)));
    }
    std::sort(endTimes.begin(), endTimes.end(), [](event &a, event &b) -> bool {return a.getMin() < b.getMin();});

    inputFile.close();

    return;
};

void department::createSchedule()
{
    currentTime = 0;
    while(endTimes.size() > 0) //While there are still employees to go through
    {
        //Update availablility (startTimes)
        updateStartTimes();
        //Take care of shifts ending
        handleEndTimes();
        //Take care of violations
        handleViolations();
        //Use the rest of the employees to fill open games in order of priority
        fillGames();
        //Move employes in lower priority games to higher priority
        consolidateGames();
        currentTime++;
    }
    for(auto x : steps)
    {
        cout << x << endl;
    }
};

void department::updateStartTimes()
{
    event tempEvent = startTimes.top();

    while(startTimes.size() > 0 && tempEvent.getMin() <= currentTime)
    {
        employee *temp = tempEvent.getEmp();
        startTimes.pop();

        if(temp->getNextRest() < breaks.size() && breaks[temp->getNextRest()].getMaxStart() + temp->getStart() < temp->getEnd()) //If haven't taken last break
        {
            violations.push_back(event(REST, temp, breaks[temp->getNextRest()].getMinStart() + currentTime, breaks[temp->getNextRest()].getMaxStart() + temp->getStart()));
        }

        availableEmployees.push(temp);

        tempEvent = startTimes.top();
    }

    sort(violations.begin(), violations.end(), [](event &a, event &b) -> bool {return a.getMin() < b.getMin();});
};

void department::handleEndTimes()
{
    while(endTimes.size() > 0 && endTimes[0].getMin() == currentTime)
    {
        if(availableEmployees.size() > 0)
        {
            steps.push_back("@" + to_string(currentTime) + " INSERT " + availableEmployees.front()->getName() + " into " +
                            endTimes[0].getEmp()->getCurrentArea()->getName() + " for " + endTimes[0].getEmp()->getName() + " to end shift");
            availableEmployees.front()->setStatus(AREA);
            availableEmployees.front()->setCurrentArea(endTimes[0].getEmp()->getCurrentArea());
            endTimes[0].getEmp()->getCurrentArea()->addEmployee();
            availableEmployees.pop();
        }
        else
        {
            steps.push_back("@" + to_string(currentTime) + " REMOVE " + endTimes[0].getEmp()->getName() + " from " + endTimes[0].getEmp()->getCurrentArea()->getName() + " to end shift");
            endTimes[0].getEmp()->getCurrentArea()->removeEmployee();
        }
        endTimes[0].getEmp()->setStatus(OFF);
        endTimes[0].getEmp()->setCurrentArea(NULL);

        endTimes.erase(endTimes.begin());
    }
};

void department::handleViolations()
{
    while(violations.size() > 0 && violations[0].getMin() <= currentTime && availableEmployees.size() > 0)
    {
        if(availableEmployees.size() > 0)
        {
            steps.push_back("@" + to_string(currentTime) + " SWAP " + availableEmployees.front()->getName() + " into " + violations[0].getEmp()->getCurrentArea()->getName() +
                            " for " + violations[0].getEmp()->getName() + "\'s " + breaks[violations[0].getEmp()->getNextRest()].getName());

            availableEmployees.front()->setStatus(AREA);
            availableEmployees.front()->setCurrentArea(violations[0].getEmp()->getCurrentArea());
            availableEmployees.pop();
        }
        else
        {
            steps.push_back("@" + to_string(currentTime) + " REMOVE " + violations[0].getEmp()->getName() + " from " + violations[0].getEmp()->getCurrentArea()->getName());
            violations[0].getEmp()->getCurrentArea()->removeEmployee();
        }

        violations[0].getEmp()->setStatus(REST);
        violations[0].getEmp()->setCurrentArea(NULL);
        startTimes.push(event(START, violations[0].getEmp(), currentTime + breaks[violations[0].getEmp()->getNextRest()].getDuration(),
                              currentTime + breaks[violations[0].getEmp()->getNextRest()].getDuration()));

        violations[0].getEmp()->incrementBreak();

        violations.erase(violations.begin());
    }
};

void department::fillGames()
{
    bool filled = false;
    while(!filled && availableEmployees.size() > 0)
    {
        filled = true;
        for(int i = 0; i < areas.size(); i++)
        {
            if(areas[i].getCurEmployees() < areas[i].getMaxEmployees())
            {
                filled = false;
                if(availableEmployees.size() > 0)
                {
                    steps.push_back("@" + to_string(currentTime) + " INSERT " + availableEmployees.front()->getName() + " into " + areas[i].getName());
                    areas[i].addEmployee();
                    availableEmployees.front()->setStatus(AREA);
                    availableEmployees.front()->setCurrentArea(&areas[i]);
                    availableEmployees.pop();
                }
            }
        }
    }
};

void department::consolidateGames()
{

};
#endif