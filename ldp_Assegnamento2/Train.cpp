/*
    Arjun Jassal - 1219611
*/
// DUMMY File
#include "Train.h"
void Train::setDelay(int d){
    timetable.update_train_time(ID, stationsCont, d, true);
}

int Train::getDelay(){
    return (currentTime/100*60+currentTime%100)-(getNextStationTimeofArrival()/100*60+getNextStationTimeofArrival()%100);
}

int getNextStationTimeofArrival(){
    TrainInfo tf=TrainTime::get_train_info(trainID);
    return tf.m_train_times.at(stationsCont+1);

}

void clock(){
    currentSpeed=0;
    stationsCont=0;
    distance+=currentSpeed/60;

    if(distance>=5 && currentSpeed==80){
        currentSpeed=maxSpeed;
    }
    if(){

    }
    StationInfo TrainLine::get_station_distances(int station_number, int starting_station, TrainType type)
}

void callTrain(StationSignal si){

}

void callTrain(StationSignal si, Track* track){

}
