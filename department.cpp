#include <sstream>
#include <fstream>
#include <algorithm>
#include "department.h"

int department::toMinutes(string in)
{
    int result;
    std::stringstream s(in);
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
    string title, startTime, endTime, maxConcecutive, maxTotal, duration, min, max; //Variables to hold the strings before conversions and saving
    char paid;
    
    ifstream inputFile;

    /********************
     * INPUT AREAS FILE *
     ********************/
    inputFile.open(areasPath);
    while(!inputFile.eof())
    {
        int max;
        getline(inputFile, title);
        inputFile.ignore(10000, '\n');
        inputFile >> max;
        inputFile.ignore(10000, '\n');
        areas.push_back(area(title, max));
    }
    inputFile.close();

    /*********************
     * INPUT BREAKS FILE *
     *********************/
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

    /************************
     * INPUT EMPLOYEES FILE *
     ************************/
    inputFile.open(employeesPath);

    int index = 0;
    while(!inputFile.eof())
    {
        string n, id, s, e; //name, employee id, start time, end time
        getline(inputFile, n);
        getline(inputFile, id);
        getline(inputFile, s);
        getline(inputFile, e);

        employees.push_back(employee(n, id, toMinutes(s), toMinutes(e)));
        startTimes.push(event(START, index, toMinutes(s), toMinutes(s)));
        endTimes.push(event(END, index, toMinutes(e), toMinutes(e)));

        index++;
    }

    inputFile.close();

/*
    //***********************************
    //* JUST FOR TESTING
    //* *********************************
    cout << "__________AREAS__________\n";
    for(int i = 0; i < areas.size(); i++)
    {
        cout << "Name: " << areas[i].getName() << endl;
        cout << "Maximum Employees: " << areas[i].getMaxEmployees() << endl;
        cout << "Current Employees: " << areas[i].getCurEmployees() << endl;
        cout << endl;
    }

    cout << "__________BREAKS__________\n";
    for(int i = 0; i < breaks.size(); i++)
    {
        cout << "Name: " << breaks[i].getName() << endl;
        cout << "Duration: " << breaks[i].getDuration() << endl;
        cout << "Min Start: " << breaks[i].getMinStart() << endl;
        cout << "Max Start: " << breaks[i].getMaxStart() << endl;
        cout << endl;
    }

    cout << "__________EMPLOYEES__________\n";
    for(int i = 0; i < employees.size(); i++)
    {
        cout << "Name: " << employees[i].getName() << endl;
        cout << "ID: " << employees[i].getId() << endl;
        cout << "Start: " << employees[i].getStart() << endl;
        cout << "End: " << employees[i].getEnd() << endl;
        cout << "Next Rest: " << employees[i].getNextRest();
        cout << endl;
    }

*/

    cout << "__________START TIMES__________\n";
    while(startTimes.size() > 0)
    {
        event curStart = startTimes.top();
        startTimes.pop();
        cout << "Employee Name: " << employees[curStart.getEmp()].getName() << endl;
        //cout << "Employee Id: " << curStart.getEmp()->getId() << endl;
        cout << "Start Time: " << curStart.getMin() << endl << endl;
    }

    cout << "__________END TIMES__________\n";
    while(endTimes.size() > 0)
    {
        event curEnd = endTimes.top();
        endTimes.pop();
        cout << "Employee Name: " << employees[curEnd.getEmp()].getName() << endl;
        cout << "Employee Id: " << employees[curEnd.getEmp()].getId() << endl;
        cout << "End Time: " << curEnd.getMin() << endl << endl;
    }

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
        //updateEndTimes();
        //Take care of violations
        //updateViolations();
        //Use the rest of the employees to fill open games in order of priority
        //fillAreas();
        currentTime++;
    }
    //sort(steps.begin(), steps.end());

    //for(auto x : steps)
    //{
    //    x.printMessage();
    //    cout << endl;
    //}
};

void department::updateStartTimes() //Moves employees starting at current time to availableEmployees, updates violations
{
    event nextStart = startTimes.top();  //The soonest start time for the next employee

    while(startTimes.size() > 0 && nextStart.getMin() <= currentTime)  //While there are employees starting at the current time
    {
        startTimes.pop();

        //The employee starting the soonest
        int       nextEmployeeIndex = nextStart.getEmp();
        employee &nextEmployee      = employees[nextEmployeeIndex];
        int       nextRestIndex     = nextEmployee.getNextRest();

        //If the next employee starting has not taken their last break and ends their
        //shift after the next violation, then push their next violation into the violations vector.
        if(nextRestIndex < breaks.size() && breaks[nextRestIndex].getMaxStart() + nextEmployee.getStart() < nextEmployee.getEnd())
        {
            violations.push_back(event(REST, nextEmployeeIndex, breaks[nextRestIndex].getMinStart() + currentTime,
                                       breaks[nextRestIndex].getMaxStart() + nextEmployee.getStart()));
        }

        //Add the employee to the queue of available employees
        availableEmployees.push(nextEmployeeIndex);

        //Update next start time for exit condition
        nextStart = startTimes.top();
    }

    //Sort the violations vector in order so that the soonest min time is first
    sort(violations.begin(), violations.end(), [](event &a, event &b) -> bool {return a.getMin() < b.getMin();});
};

void department::updateEndTimes() //Removes employees if their shift is ending at the current time by closing their game
                                  //or moving another employee from a lower ranked game if available.
{
    event nextEndTime = endTimes.top();
    employee &endingEmp = employees[nextEndTime.getEmp()];

    while(endTimes.size() > 0 && nextEndTime.getMin() == currentTime) //While at least one employee's shift is ending at the current time
    {
        endTimes.pop();

        if(availableEmployees.size() > 0)  //If there are available employees, swap for the employees ending their shift
        {
            int nextEmpIndex = availableEmployees.front();
            availableEmployees.pop();
            employee &nextEmp = employees[nextEmpIndex];
            string newStep = "@" + to_string(currentTime) + " INSERT " + nextEmp.getName() + " into " +
                             endingEmp.getCurrentArea()->getName() + " for " + endingEmp.getName() + " to end shift";
            steps.push_back(step(currentTime, INSERT, newStep, availableEmployees.front(), employees[nextEndTime.getEmp()].getCurrentArea()));
            nextEmp.setStatus(AREA);
            nextEmp.setCurrentArea(nextEndTime.getEmp()->getCurrentArea());
        }
        else //Else, try and find someone else to replace them
        {
            employee *nextEmp = findNextEmployee(); //If no employees found, error
            if(!nextEmp)
                cout << "Unknown error, CODE 1\n";
            else if(nextEmp->getCurrentArea() != nextEndTime.getEmp()->getCurrentArea()) //If another employee can cover
            {
                string newStep = "@" + to_string(currentTime) + " MOVE " + nextEmp->getName() + " from " + nextEmp->getCurrentArea()->getName()
                                 + " to " + nextEndTime.getEmp()->getCurrentArea()->getName() + " to let out " + nextEndTime.getEmp()->getName() + " for END OF SHIFT";
                nextEmp->getCurrentArea()->removeEmployee();
                if(nextEmp->getCurrentArea()->getCurEmployees() < 1)
                    newStep += ", close " + nextEmp->getCurrentArea()->getName();

                steps.push_back(step(currentTime, MOVE, newStep, nextEmp, nextEndTime.getEmp()->getCurrentArea(), nextEndTime.getEmp(), nextEmp->getCurrentArea()));
                nextEmp->setCurrentArea(nextEndTime.getEmp()->getCurrentArea());
            }
            else // If no employee can cover
            {
                string newStep = "@" + to_string(currentTime) + " REMOVE " + nextEndTime.getEmp()->getName() + " from " + nextEndTime.getEmp()->getCurrentArea()->getName()
                                 + " for END OF SHIFT";
                nextEndTime.getEmp()->getCurrentArea()->removeEmployee();
                if(nextEndTime.getEmp()->getCurrentArea()->getCurEmployees() < 1)
                    newStep += ", close " + nextEndTime.getEmp()->getCurrentArea()->getName();                
            }
        }

        //Employee has no area and is off
        nextEndTime.getEmp()->setStatus(OFF);
        nextEndTime.getEmp()->setCurrentArea(NULL);

        nextEndTime = endTimes.top();
    }

};

void department::updateViolations()
{
    while(violations.size() > 0 && violations[0].getMin() <= currentTime)
    {
        if(availableEmployees.size() > 0)
        {
            string newStep = "@" + to_string(currentTime) + " SWAP " + availableEmployees.front()->getName() + " into " + violations[0].getEmp()->getCurrentArea()->getName() +
                            " for " + violations[0].getEmp()->getName() + "\'s " + breaks[violations[0].getEmp()->getNextRest()].getName();
            steps.push_back(step(currentTime, SWAP, newStep, availableEmployees.front(), violations[0].getEmp()->getCurrentArea(), violations[0].getEmp()));

            availableEmployees.front()->setStatus(AREA);
            availableEmployees.front()->setCurrentArea(violations[0].getEmp()->getCurrentArea());
            availableEmployees.pop();
        }
        else
        {
            string newStep = "@" + to_string(currentTime) + " REMOVE " + violations[0].getEmp()->getName() + " from " +
                             violations[0].getEmp()->getCurrentArea()->getName() + " for " + breaks[violations[0].getEmp()->getNextRest()].getName();
            steps.push_back(step(currentTime, REMOVE, newStep, violations[0].getEmp(), violations[0].getEmp()->getCurrentArea()));
            violations[0].getEmp()->getCurrentArea()->removeEmployee(); //ERROR HERE
        }

        violations[0].getEmp()->setStatus(REST);
        violations[0].getEmp()->setCurrentArea(NULL);
        startTimes.push(event(START, violations[0].getEmp(), currentTime + breaks[violations[0].getEmp()->getNextRest()].getDuration(),
                              currentTime + breaks[violations[0].getEmp()->getNextRest()].getDuration()));

        violations[0].getEmp()->incrementBreak();

        violations.erase(violations.begin());
    }

};

void department::fillAreas()
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
                    string newStep = "@" + to_string(currentTime) + " INSERT " + availableEmployees.front()->getName() + " into " + areas[i].getName(); //ERROR HERE
                    steps.push_back(step(currentTime, INSERT, newStep, availableEmployees.front(), &areas[i]));
                    areas[i].addEmployee();
                    availableEmployees.front()->setStatus(AREA);
                    availableEmployees.front()->setCurrentArea(&areas[i]);
                    availableEmployees.pop();
                }
            }
        }
    }
};

employee* department::findNextEmployee() //Find the next currently working employee to cover for a leaving employee
{
    if(areas.size() > 0 && areas[areas.size() - 1].getCurEmployees() == 0) //If not all games are filled, find the employee at the lowest ranked game
        for(int i = areas.size() - 1; i >=0; i--)
            if(areas[i].getCurEmployees() > 0)
                for(auto &x : employees)
                    if(x.getCurrentArea() == &areas[i])
                        return &x;
    else
    {
        float highestPercentage = 0;
        area *highestPercentageArea = NULL;

        for(auto &x : areas)
        {
            if(x.getCurEmployees() > 1)
            {
                float currPercentage = float(x.getCurEmployees())/x.getMaxEmployees();
                if(currPercentage > highestPercentage)
                {
                    highestPercentage = currPercentage;
                    highestPercentageArea = &x;
                }
            }
        }
        if(highestPercentageArea)
            for(auto &x : employees)
                if(x.getCurrentArea() == highestPercentageArea)
                    return &x;
        else
            for(auto &x : employees)
                if(x.getCurrentArea() == &areas[areas.size()-1]);
                    return &x;
    }
    return NULL;
}
