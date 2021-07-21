#include "department.h"
#include <iostream>

using namespace std;

int main()
{
    department d;
    d.importData("Data\areas.dat", "Data\breaks.dat", "Data\employees.dat");
    //d.createSchedule();

    return 0;
}