#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>

class employee
{
public:
    void setName(string &newName) {name = newName;};
    void setId(string &newId) {id = newId;};
    void setStart(int newStart) {start = newStart;};
    void setEnd(int newEnd) {end = newEnd;};
    
    string getName() {return name;};
    string getId() {return id;};
    short getStart() {return start;};
    short getEnd() {return end;};

private:
    string name;
    string id;
    short start;
    short end;
};

#endif