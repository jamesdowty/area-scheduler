#ifndef REST_H
#define REST_H

#include <string>

using namespace std;

class rest
{
public:
    rest(string, int, int, int, bool);
    string getName() {return name;};
    int getDuration() {return duration;};
    int getMinStart() {return minStart;};
    int getMaxStart() {return maxStart;};
private:
    string name;
    int duration;
    int minStart;
    int maxStart;
    bool paid;
};

rest::rest(string n, int dur, int min, int max, bool p)
{
    name = n;
    duration = dur;
    minStart = min;
    maxStart = max;
    paid = p;
};

#endif