#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please
#endif
#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool scheduleHelper(const AvailabilityMatrix& avail, const size_t dailyNeed, const size_t maxShifts, DailySchedule& sched, vector<size_t>& shifts);

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below
    
    //used to keep track of how many shifts each worker has used
    vector<size_t> shifts(avail.at(0).size());

    //create the first day with 0 workers scheduled
    sched.push_back(vector<Worker_T>());

    if(scheduleHelper(avail, dailyNeed, maxShifts, sched, shifts)){
        return true;
    }
    return false;
}


bool scheduleHelper(const AvailabilityMatrix& avail, const size_t dailyNeed, const size_t maxShifts, DailySchedule& sched, vector<size_t>& shifts){
    //base case
    //if the schedule size (amount of days) is the same size as the availability matrix
    //and if we filled the last day with the needed number of workers,
    //then we found a valid schedule
    if(sched.size() == avail.size() && sched.at(sched.size() - 1).size() == dailyNeed) {
        return true;
    }

    //current day that we are scheduling
    int day = sched.size() - 1;
    Worker_T worker;

    //if it is a new day, start with worker 0
    //in other words, if there are no workers scheduled for the day, start with worker 0
    if(sched.at(sched.size() - 1).size() == 0){
        worker = 0;
    }
    //if it is not a new day, start with the last worker scheduled + 1
    else {
        //get the worker on the latest day 
        //latest day = sched.at(sched.size() - 1)
        //last worker = latest day.at(latest day.size() - 1)
        worker = sched.at(sched.size() - 1).at(sched.at(sched.size() - 1).size() - 1) + 1;
    }
    
    //since we are using worker for our looping condition and it was already initialized, 
    //we do not have to write an initialization in the for loop
    for(/*not needed*/; worker < avail[day].size(); ++worker){
        
        //if the worker is available to work that day and that they have not exceeded the max shifts
        if(avail.at(day).at(worker) != 0 && shifts.at(worker) != maxShifts){

            //add the worker to the schedule for the day
            sched.at(day).push_back(worker);
            //increase the workers shifts
            shifts.at(worker)++;

            //if we have scheduled the max amount of employees for the day, add another, blank day
            if(sched.at(day).size() == dailyNeed) {
                if(sched.size() != avail.size()){
                    sched.push_back(vector<Worker_T>());
                }
            } 

            //try the next addition to the schedule
            if(scheduleHelper(avail, dailyNeed, maxShifts, sched, shifts)){
                return true;
            }
            
            //if we backtracked here, undo all changes we did previously

            //if we inserted the last worker for the day, that means we created a new day previusly
            //so we have to pop the schedule
            if(sched.at(day).size() == dailyNeed) {
                sched.pop_back();
            }

            //reduce the shifts of the worker
            shifts.at(worker)--;
            //remove the worker that we scheduled for that day
            sched.at(day).pop_back();
        }
    }
    
    //returns false only when we have exhausted every possible solution
    return false;
}