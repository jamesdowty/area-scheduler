#include "step.h"

using namespace std;

step::step(int t, STEP_TYPE st, string m, employee *ei, area *w, employee *eo, area *wf)
{
    time = t;
    stepType = st;
    message = m;
    employeeIn = ei;
    where = w;
    employeeOut = eo;
    whereFrom = wf;
};

step::step(int t, STEP_TYPE st, string m, employee *ei, area *w, employee *eo)
{
    time = t;
    stepType = st;
    message = m;
    employeeIn = ei;
    where = w;
    employeeOut = eo;
    whereFrom = NULL;
};

step::step(int t, STEP_TYPE st, string m, employee *ei, area *w)
{
    time = t;
    stepType = st;
    message = m;
    employeeIn = ei;
    where = w;
    employeeOut = NULL;
    whereFrom = NULL;
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
