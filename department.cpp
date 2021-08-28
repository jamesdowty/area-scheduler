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

    //***********************************
    //* JUST FOR TESTING
    //* *********************************
    /*
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
*/
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
        updateEndTimes();
        //Take care of violations
        updateViolations();
        //Use the rest of the employees to fill open games in order of priority
        fillAreas();
        currentTime++;
    }
    sort(steps.begin(), steps.end());
    for(auto x : steps)
    {
        x.printMessage();
        cout << endl;
    }
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

void department::updateEndTimes() //Removes employees if their shift is ending at the current time by closing their game or moving another employee from a lower ranked game if available.
{
    while(endTimes.size() > 0 && endTimes.top().getMin() == currentTime) //While at least one employee's shift is ending at the current time
    {
        event nextEndTime = endTimes.top();
        endTimes.pop();
        int endingEmpIndex = nextEndTime.getEmp();
        employee &endingEmp = employees[endingEmpIndex];
        int endingAreaIndex = endingEmp.getCurrentArea();
        area &endingArea = areas[endingAreaIndex];

        if(availableEmployees.size() > 0)  //If there are available employees, swap for the employees ending their shift
        {
            int nextEmpIndex = availableEmployees.front();
            employee &nextEmp = employees[nextEmpIndex];
            availableEmployees.pop();

            string newStep = "@" + to_string(currentTime) + " INSERT " + nextEmp.getName() + " into " +
                             endingArea.getName() + " for " + endingEmp.getName() + " to end shift";
            steps.push_back(step(currentTime, INSERT, newStep, availableEmployees.front(), employees[nextEndTime.getEmp()].getCurrentArea()));
            nextEmp.setStatus(AREA);
            nextEmp.setCurrentArea(endingAreaIndex);
        }
        else //Else, try and find someone else to replace them
        {
            int nextEmpIndex = findNextEmployee(); //If no employees found, error
            employee &nextEmp = employees[nextEmpIndex];
            if(nextEmpIndex == -1)
                cout << "Unknown error, CODE 1\n";

            else if(nextEmp.getCurrentArea() != endingAreaIndex && nextEmp.getEnd() != endingEmp.getEnd()) //If another employee can cover
            {
                string newStep = "@" + to_string(currentTime) + " MOVE " + nextEmp.getName() + " from " + areas[nextEmp.getCurrentArea()].getName()
                                 + " to " + areas[endingAreaIndex].getName() + " to let out " + endingEmp.getName() + " for END OF SHIFT";
                --areas[nextEmp.getCurrentArea()];
                if(areas[nextEmp.getCurrentArea()].getCurEmployees() < 1)
                    newStep += ", close " + areas[nextEmp.getCurrentArea()].getName();
                steps.push_back(step(currentTime, MOVE, newStep, nextEmpIndex, endingAreaIndex, nextEndTime.getEmp(), nextEmp.getCurrentArea()));
                nextEmp.setCurrentArea(endingAreaIndex);
            }

            else // If no employee can cover
            {
                string stepString = "@" + to_string(currentTime) + " REMOVE " + nextEmp.getName() + " from " + endingArea.getName()
                                 + " for END OF SHIFT";
                --endingArea;
                if(endingArea.getCurEmployees() < 1)
                    stepString += ", close " + endingArea.getName(); 

                steps.push_back(step(currentTime, REMOVE, stepString, endingEmpIndex, endingAreaIndex));             
            }
        }
        //Employee has no area and is off
        endingEmp.setStatus(OFF);
        endingEmp.setCurrentArea(-1);
    }

};

/*
1. read in data
2. Find all newly available employees
3. Handle end times
4. Ind handle violations, follow these steps:
    a. Find all mandatory violations
    b. If employees available, fill mandatory violations
    c. If no employees available, do dfs to find best place to remove mandatory emp, KEEP CHAIN GOING UNTIL CURRENT TIME, DO NOT CHANGE currentTime!
    a. Find all currently elligible violations (if employees are still available)
    b. If available employees, do dfs to find most efficient paths and assign employee to that violation
    c. If at the end of a violation, do another dfs to find the best time to remove employee, remove that
       employee at the optimal time and set the current time back the the start time of that violation
*/
void department::updateViolations() 
{
    int violationIndex = 0;
    vector<int> eligibleViolatons;
    vector<int> mandatoryViolations;
    
    //Finding eligible violations and pushing indices onto vector
    while(violationIndex < violations.size() && violations[violationIndex].getMin() <= currentTime)
    {
        if(violations[violationIndex].getMax() < currentTime)
            eligibleViolatons.push_back(violationIndex);
        else if(violations[violationIndex].getMax() == currentTime)
            mandatoryViolations.push_back(violationIndex);
        
        ++violationIndex;
    }

    while(mandatoryViolations.size() > 0)
    {
        if(availableEmployees.size() > 0)
        {
            //Take care of violation with an available employee
            int currViolationIndex = mandatoryViolations.back();
            mandatoryViolations.pop_back();

            int currEmployeeIndex = violations[currViolationIndex].getEmp();
            employee &currEmployee = employees[currEmployeeIndex];

            int nextEmployeeIndex = availableEmployees.front();
            employee &nextEmployee = employees[nextEmployeeIndex];
            availableEmployees.pop();

            string newStep = "@" + to_string(currentTime) + " INSERT " + nextEmployee.getName() + " into " +
                             areas[currEmployee.getCurrentArea()].getName() + " for " + currEmployee.getName() + "\'s " + breaks[currEmployee.getNextRest()].getName();
            steps.push_back(step(currentTime, SWAP, newStep, nextEmployeeIndex, currEmployee.getCurrentArea(), currEmployeeIndex));

            nextEmployee.setCurrentArea(currEmployee.getCurrentArea());
            nextEmployee.setStatus(AREA);

            currEmployee.setCurrentArea(-1);
            currEmployee.setStatus(REST);
            int nextStartTime = currentTime + breaks[currEmployee.getNextRest()].getDuration();
            startTimes.push(event(REST, currEmployeeIndex, nextStartTime, nextStartTime));
            currEmployee.incrementBreak();
        }
        else
        {
            //DFS Unknown Time
            int finalStartTime = 0;
            int finalViolationIndex = 0;
            int maxBreakCount = 0;

            for(int currentSearch : eligibleViolatons)
            {
                event &currViolation = violations[currentSearch];
                int breakCount = 0;
                int startTime = currViolation.getMin();
                int totalDuration = 0;
                unordered_map<int, bool> visited;

                dfs(currentSearch, currViolation.getMin(), currViolation.getMax(), visited, startTime, breakCount, maxBreakCount, totalDuration);

                if(startTime > finalStartTime)
                {
                    finalStartTime = startTime;
                    finalViolationIndex = currentSearch;
                }
            }

            //REMOVE EMPLOYEE FROM GAME TO REPLACE LEAVING EMPLOYEE
            int currEmployeeIndex = violations[finalViolationIndex].getEmp();
            employee &currEmployee = employees[currEmployeeIndex];
/*
            int nextEmployeeIndex = availableEmployees.front();
            employee &nextEmployee = employees[nextEmployeeIndex];
            availableEmployees.pop();

            string newStep = "@" + to_string(finalStartTime) + " INSERT " + nextEmployee.getName() + " into " +
                            areas[currEmployee.getCurrentArea()].getName() + " for " + currEmployee.getName() + "\'s " + breaks[currEmployee.getNextRest()].getName();
            steps.push_back(step(currentTime, SWAP, newStep, nextEmployeeIndex, currEmployee.getCurrentArea(), currEmployeeIndex));

            nextEmployee.setCurrentArea(currEmployee.getCurrentArea());
            nextEmployee.setStatus(AREA);

            currEmployee.setCurrentArea(-1);
            currEmployee.setStatus(REST);
            int nextStartTime = currentTime + breaks[currEmployee.getNextRest()].getDuration();
            startTimes.push(event(REST, currEmployeeIndex, nextStartTime, nextStartTime));
            currEmployee.incrementBreak();
*/
        }
    }

    while(availableEmployees.size() > 0 && eligibleViolatons.size() > 0)
    {
        //DFS Current Time
        int finalStartTime = 0;
        int finalViolationIndex = 0;
        int maxBreakCount = 0;

        for(int currentSearch : eligibleViolatons)
        {
            event &currViolation = violations[currentSearch];
            int breakCount = 0;
            int startTime = currViolation.getMin();
            int totalDuration = 0;
            unordered_map<int, bool> visited;

            dfs(currentSearch, currentTime, currentTime, visited, startTime, breakCount, maxBreakCount, totalDuration);

            if(startTime > finalStartTime)
            {
                finalStartTime = startTime;
                finalViolationIndex = currentSearch;
            }
        }

        //REPLACE EMPLOYEE FROM VIOLATION INDEX
        int currEmployeeIndex = violations[finalViolationIndex].getEmp();
        employee &currEmployee = employees[currEmployeeIndex];

        int nextEmployeeIndex = availableEmployees.front();
        employee &nextEmployee = employees[nextEmployeeIndex];
        availableEmployees.pop();

        string newStep = "@" + to_string(currentTime) + " INSERT " + nextEmployee.getName() + " into " +
                        areas[currEmployee.getCurrentArea()].getName() + " for " + currEmployee.getName() + "\'s " + breaks[currEmployee.getNextRest()].getName();
        steps.push_back(step(currentTime, SWAP, newStep, nextEmployeeIndex, currEmployee.getCurrentArea(), currEmployeeIndex));

        nextEmployee.setCurrentArea(currEmployee.getCurrentArea());
        nextEmployee.setStatus(AREA);

        currEmployee.setCurrentArea(-1);
        currEmployee.setStatus(REST);
        int nextStartTime = currentTime + breaks[currEmployee.getNextRest()].getDuration();
        startTimes.push(event(REST, currEmployeeIndex, nextStartTime, nextStartTime));
        currEmployee.incrementBreak();
    }


};

void department::fillAreas()
{
    bool filled = false;
    int  empsPerArea = 1;
    while(!filled && availableEmployees.size() > 0)
    {
        filled = true;
        for(int i = 0; i < areas.size(); i++)
        {
            if(areas[i].getCurEmployees() < areas[i].getMaxEmployees())
            {
                filled = false;
                if(areas[i].getCurEmployees() < empsPerArea && availableEmployees.size() > 0)
                {
                    int nextEmployeeIndex = availableEmployees.front();
                    employee &nextEmployee = employees[nextEmployeeIndex];
                    availableEmployees.pop();
                    string stepString = "@" + to_string(currentTime) + " INSERT " + nextEmployee.getName() + " into " + areas[i].getName();
                    steps.push_back(step(currentTime, INSERT, stepString, nextEmployeeIndex, i));

                    nextEmployee.setStatus(AREA);
                    nextEmployee.setCurrentArea(i);
                    ++areas[i];
                }
            }
        }

        ++empsPerArea;
    }
};

int department::findNextEmployee() //Find the next currently working employee to cover for a leaving employee
{
    if(areas.size() > 0 && areas[areas.size() - 1].getCurEmployees() == 0) //If not all games are filled, find the employee at the lowest ranked game
    {
        for(int i = areas.size() - 1; i >=0; i--)
        {
            if(areas[i].getCurEmployees() > 0)
            {
                for(int j = 0; j < employees.size(); j++)
                {
                    if(employees[j].getCurrentArea() == i)
                    {
                        return j;
                    }
                }
            }
        }
    }
    else  //If all games have at least one employee
    {
        vector<int> multiEmp;
        for(int i = 0; i < areas.size(); i++)
        {
            if(areas[i].getCurEmployees() > 1)
            {
                multiEmp.push_back(i);
            }
        }

        float highestPercentage = 0;
        int   highestPercentageArea = -1;

        if(multiEmp.size() == 0)
        {
            highestPercentageArea = areas.size() - 1;
        }
        else
        {
            //Find game with highest percentage employeed
            for(int i = 0; i < multiEmp.size(); i++)
            {
                float currPercentage = float(areas[multiEmp[i]].getCurEmployees())/areas[multiEmp[i]].getMaxEmployees();
                if(currPercentage > highestPercentage)
                {
                    highestPercentage = currPercentage;
                    highestPercentageArea = multiEmp[i];
                }
            }
        }

        //Pull from that game
        for(int i = 0; i < employees.size(); i++)
        {
            if(employees[i].getCurrentArea() == highestPercentageArea)
            {
                return i;
            }
        }
    }
    return -1;
};

void department::dfs(int searchIndex, int minTime, int maxTime, unordered_map<int, bool> visited, int &startTime, int &breakCount, int &maxCount, int totalDuration)
{
    visited[currentSearch] = true;
    startTime = minTime-totalDuration;
    breakCount++;
    int currentBreakDuration = violations[currentSearch].getBreakIndex()].getDuration();
    totalDuration += currentBreakDuration

    minTime += breaks[violations[currentSearch].getBreakIndex()].getDuration();
    maxTime += breaks[violations[currentSearch].getBreakIndex()].getDuration();

    for(int i = 0; i < violations.size(); i++)
    {
        event &currViolation = violations[i];
        if(!visited[i] && currViolation.getMax() >= minTime && currViolation.getMin() <= maxTime)
        {
            dfs(i, max(minTime, currViolation.getMin), min(maxTime, currViolation.getMax()), visited, startTime, breakCount, maxCount, totalDuration);
        }
    }
    if(breakCount > maxCount)
    {
        maxCount = breakCount;
        startTime = minTime;
    }
    return;
}