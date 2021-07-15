/******************************************
 * Program:      Area Scheduler           *
 * File:         eventCatagory.h          *
 * Author:       James Dowty              *
 * Last Updated: 07/14/2021               *
 * -------------------------------------- *
 * Enumorated type for catagory/status    *
 * ****************************************/

#ifndef EVENT_CATAGORY_H
#define EVENT_CATAGORY_H

enum eventCatagory{
    START, //Starting shift
    END,   //Ending shift
    REST,  //Break
    AREA,  //At area
    OFF,   //Outside of shift interval
    MISC   //Available but not at a game
};

#endif