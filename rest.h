/******************************************
 * Program:      Area Scheduler           *
 * File:         rest.h                   *
 * Author:       James Dowty              *
 * Last Updated: 07/14/2021               *
 * -------------------------------------- *
 * The rest class holds the data for each *
 * break an employee is required to take. *
 * ****************************************/

#ifndef REST_H
#define REST_H

#include <string>

class rest
{
public:
    /*********************************************************
     * Description:   Constructor for a rest object.
     * Precondition:  none
     * Postcondition: A rest object is instatiated with the
     *                specified values.
     * ******************************************************/
    rest(std::string,   //Name of break
         int,           //Duration of break (in minutes)
         int minStart,  //Minimum start of break (in minutes).
                        //Ex: 30 minutes must be worked since last break before starting this one.
         int maxStart,  //Maximum start of break (in minutes).
                        //Ex: No more than 180 minutes can be worked since last break before starting this one.
         bool paid);    //Whether or not the break is paid (T=paid/F=unpaid)

    /*********************************************************
     * Description:   Accessor for the name of the break.
     * Precondition:  none
     * Postcondition: Name of the break is returned.
     * ******************************************************/
    std::string getName();

    /*********************************************************
     * Description:   Accessor for the duration of the break.
     * Precondition:  none
     * Postcondition: Duration of the break is returned in
     *                minutes.
     * ******************************************************/
    int getDuration();

    /*********************************************************
     * Description:   Accessor for the minimum start time
     *                of the break.
     * Precondition:  none
     * Postcondition: Minimum start time of the break is
     *                returned in minutes.
     * ******************************************************/
    int getMinStart();

    /*********************************************************
     * Description:   Accessor for the maximum start time
     *                of the break.
     * Precondition:  none
     * Postcondition: Maximum start time of the break is
     *                returned in minutes.
     * ******************************************************/
    int getMaxStart();

private:
    std::string name; //Name of the break
    int duration;     //Duration of the break in minutes
    int minStart;     //Minimum start time of the break in minutes
    int maxStart;     //Maximum start time of the break in minutes
    bool paid;        //TRUE if the break is paid, FALSE if unpaid
};

#endif