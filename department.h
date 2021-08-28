/******************************************
 * Program:      Area Scheduler           *
 * File:         department.h             *
 * Author:       James Dowty              *
 * Last Updated: 07/20/2021               *
 * -------------------------------------- *
 * This class holds an entire department  *
 * along with the functions that create   *
 * and print schedules                    *
 * ****************************************/

#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include <string>
#include <vector>
#include <queue>
#include "employee.h"
#include "area.h"
#include "event.h"
#include "rest.h"
#include "step.h"

class department
{
public:

    /*********************************************************
     * Description:   Imports data from files for department
     * Precondition:  Data files exist for areas, breaks, and
     *                employees
     * Postcondition: All of the data for the department is
     *                stored in variables
     * ******************************************************/
    void importData(string, //Path for areas file
                    string, //Path for breaks file
                    string);//Path for employees file

    /*********************************************************
     * Description:   Creates a schedule for the department
     *                and stores the steps in the steps vector
     * Precondition:  Data for the department has been read in
     * Postcondition: Steps vector is populated with the best
     *                way to manage the department
     * ******************************************************/
    void createSchedule();
    void printSteps();
    void printEmployeeSchedule();
    void printGameSchedule();
    
private:
    vector<employee> employees;
    vector<area>     areas;
    vector<event>    violations;
    vector<rest>     breaks;
    vector<step>     steps;
    queue<int>       availableEmployees;
    queue<int>       misc;
    priority_queue<event, vector<event>, greater<event>> startTimes;
    priority_queue<event, vector<event>, greater<event>>   endTimes;
    int       maxConcecutiveTime;
    int       maxTotalTime;
    int       parkOpen;
    int       parkClose;
    int       currentTime;
    int       toMinutes(string);
    void      updateStartTimes();
    void      updateViolations();
    void      updateEndTimes();
    void      fillAreas();
    int       findNextEmployee();
    int       miscEmployees = 0;
    int       dfs(int, int, int, unordered_map<int, bool>, int&, int&, int&, int);
};

#endif