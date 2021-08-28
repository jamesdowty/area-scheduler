#include "event.h"

using namespace std;

event::event(eventCatagory e, int em, int mi, int ma)
{
    catagory = e;
    breakIndex = 0;
    emp = em;
    min = mi;
    max = ma;
}

short event::getMin() const
{
    return min;
}

short event::getMax()
{
    return max;
}

int event::getEmp()
{
    return emp;
}

bool event::operator>(const event &a) const
{
    return min > a.getMin();
};

int  event::getBreakIndex()
{
    return breakIndex;
}