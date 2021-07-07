#ifndef EVENT_H
#define EVENT_H

#include <string>

class event
{
public:
    void setName(string &newName) {name = newName;};
    void setEmployee(string &newEmployee) {employee = newEmployee;};
    void setMin(int newMin) {min = newMin;};
    void setMax(int newMax) {max = newMax;};
    
    string getName() {return name;};
    string getEmployee() {return employee;};
    short getMin() {return min;};
    short getMax() {return max;};
private:
    string name;
    string employee;
    short min;
    short max;
};

#endif