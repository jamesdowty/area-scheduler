#ifndef AREA_H
#define AREA_H

#include <string>

using namespace std;

class area
{
public:
    area(string, short);
    void setName(string newName);
    void setMaxEmployees(int newMax);
    void addEmployee();
    void removeEmployee();
    area& operator++();
    area& operator--();
    string getName();
    short getMaxEmployees();
    short getCurEmployees();
private:
    string name;
    short maxEmployees;
    short curEmployees;
};


#endif