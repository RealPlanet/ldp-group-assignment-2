// @author Arjun Jassal, 1219611

#include "Train.h"


void Train::setDelay(int d){
    timetable.update_train_time(ID, stationsCont, d, true);
}

void Train::setNextStation(Station* next){
    
}
