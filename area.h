#ifndef AREA_H
#define AREA_H

#include <string>
#include "employee.h"

using namespace std;

class area
{
public:
    area(string, short, short);
    void setName(string &newName) {name = newName;};
    void setMinEmployees(int newMin) {minEmployees = newMin;};
    void setMaxEmployees(int newMax) {maxEmployees = newMax;};
    void removeEmployee();
    void addEmployee();
    
    string getName() {return name;};
    short getMinEmployees() {return minEmployees;};
    short getMaxEmployees() {return maxEmployees;};
    short getCurEmployees() {return curEmployees;};
private:
    string name;
    short minEmployees;
    short maxEmployees;
    short curEmployees;
};

area::area(string newName, short newMin, short newMax)
{
    name = newName;
    minEmployees = newMin;
    maxEmployees = newMax;
    curEmployees = 0;
};

void area::removeEmployee()
{
    curEmployees--;
}

void area::addEmployee()
{
    curEmployees++;
}

#endif