#include "area.h"

area::area(string newName, short newMax)
{
    name = newName;
    maxEmployees = newMax;
    curEmployees = 0;
};

short area::getCurEmployees()
{
    return curEmployees;
};

short area::getMaxEmployees()
{
    return maxEmployees;
};

string area::getName()
{
    return name;
};

void area::removeEmployee()
{
    curEmployees--;
};

void area::addEmployee()
{
    curEmployees--;
};

void area::setMaxEmployees(int newMax)
{
    maxEmployees = newMax;
};

void area::setName(string newName)
{
    name = newName;
};

area& area::operator--()
{
    curEmployees--;
    return *this;
}

area& area::operator++()
{
    curEmployees++;
    return *this;
}
