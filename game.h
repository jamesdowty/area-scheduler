#ifndef GAME_H
#define GAME_H

#include <string>

class game
{
public:
    void setName(string &newName) {name = newName;};
    void setMin(int newMin) {min = newMin;};
    void setMax(int newMax) {max = newMax;};
    
    string getName() {return name;};
    short getMin {return min;};
    short getMax {return max;};
private:
    string name;
    short min;
    short max;
};

#endif