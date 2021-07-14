#include "department.h"
#include <iostream>

using namespace std;

int main()
{
    department d;
    d.importData("areas.dat", "breaks.dat", "employees.dat");

    return 0;
}