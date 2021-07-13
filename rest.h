#ifndef REST_H
#define REST_H

class rest
{
public:
    rest(string, int, int, int, bool);
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