#include "step.h"

using namespace std;

step::step(int t, STEP_TYPE st, string m, int ei, int w, int eo, int wf)
{
    time = t;
    stepType = st;
    message = m;
    employeeIn = ei;
    where = w;
    employeeOut = eo;
    whereFrom = wf;
};

step::step(int t, STEP_TYPE st, string m, int ei, int w, int eo)
{
    time = t;
    stepType = st;
    message = m;
    employeeIn = ei;
    where = w;
    employeeOut = eo;
    whereFrom = -1;
};

step::step(int t, STEP_TYPE st, string m, int ei, int w)
{
    time = t;
    stepType = st;
    message = m;
    employeeIn = ei;
    where = w;
    employeeOut = -1;
    whereFrom = -1;
};

void step::printMessage()
{
    cout << message;
};

int step::getTime() const
{
    return time;
};

bool step::operator<(const step &x)
{
    return time < x.getTime();
};
