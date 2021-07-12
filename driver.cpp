#include "department.h"
#include <iostream>

using namespace std;

int main()
{
    department d;
    d.importData("areas.dat", "employees.dat", "breaks.dat");

    return 0;
}