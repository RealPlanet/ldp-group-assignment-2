#ifndef train_h
#define train_h

// @author Arjun Jassal, 1219611
#include "generics.h"
#include "..\include\Station.h"
#include "..\include\TrainTime.h"
#include "..\include\Track.h"
#include <ctgmath>

static int getTrainSpeed(TrainType elem);

static int timeConversion(int t){
    return (t/100*60+t%100);
}

class Train{
    public:
        class InvalidTrainType : std::exception{};

        Train(int ID, TrainType t, TrainDirection dir, TrainLine* l, TrainTime* time)
            : trainID{ID}, trainType{t}, direction{dir}, line{l}, trainTime{time} {
                maxSpeed=getTrainSpeed(t);
                currentSpeed=0;
                visitedStations=0;
                distance=0;
        }

        //GET
        TrainDirection getTrainDirection() {return direction;}
        TrainType getTrainType() {return trainType;}
        int getMaxSpeed() {return maxSpeed;}
        int getTrainID() {return trainID;}
		Track* getTrack() {return track;}
        float getDistance() {return distance;}
        bool isEndline() {return endline;}

        //COMUNICAZIONE CON STAZIONE
        virtual void callTrain(StationSignal si) = 0;

        void callTrain(StationSignal si, Track* t){                                             //chiamata alla stazione con binario
            parking=false;                                                                      //treno non si ferma in parcheggio
            track=t;                                                                            //assegna il binario
        }

        //CLOCK TRENO
        virtual void clock(int t)=0;

        //RITARDO TRENI
        int getDelay(){
            int t=trainTime->get_train_info(trainID).m_train_times.at((size_t)visitedStations+1);        //differenza tra orario corrente e previsto
            return time-timeConversion(t);
        }

        //ORARIO DI ARRIVO PREVISTO ALLA STAZIONE SUCCESSIVA
        int getNextArrivalTime(){
            return trainTime->get_train_info(trainID).m_train_times.at((size_t)visitedStations+1);       //ritorna l'orario previsto alla stazione+1(successiva)
        }

        int getArrivalTime(){
            return trainTime->get_train_info(trainID).m_train_times.at(visitedStations);
        }


    protected:
        int trainID;                                                //ID del treno
        TrainType trainType;                                        //tipo del treno
        TrainDirection direction;                                   //direzione del treno

        int timer=-1;                                               //timer di sosta in stazione

        bool startline=true;                                        //controllo se è la prima stazione della linea
        bool endline=false;                                         //controllo se è l'ultima stazione della linea

        bool parking=false;                                         //controllo se va in parcheggio o no

        int visitedStations;                                        //contatore delle stazioni visitate

        Track* track = nullptr;                                     //binario associato in arrivo alla stazione

        TrainLine* line;                                            //linea associata al treno
        TrainTime* trainTime;                                       //tabella degli orari associata al treno

        int maxSpeed;                                               //velocità massima
        int currentSpeed;                                           //velocità di crociera

        int time=0;

        float distance;                                             //distanza totale percorsa da 0
        Station* prevStation=nullptr;                               //stazione precedente
        Station* nextStation=nullptr;                               //stazione successiva
};

class RegionalTrain : public Train{
    public:
        RegionalTrain(int ID, TrainDirection dir, TrainLine* l, TrainTime* time);
		void callTrain(StationSignal si);
        void clock(int t);
};

class HighSpeedTrain : public Train{
    public:
        HighSpeedTrain(int ID, TrainDirection dir, TrainLine* l, TrainTime* time);
		void callTrain(StationSignal si);
        void clock(int t);
    protected:
        Station* prevMainStation=nullptr;                                   //stazione principale precedente
        Station* nextMainStation=nullptr;                                   //stazione principale successiva
};

class SuperHighSpeedTrain : public Train{
    public:
        SuperHighSpeedTrain(int ID, TrainDirection dir, TrainLine* l, TrainTime* time);
		void callTrain(StationSignal si);
        void clock(int t);
    protected:
        Station* prevMainStation=nullptr;                                   //stazione principale precedente
        Station* nextMainStation=nullptr;                                   //stazione principale successiva
};

static int getTrainSpeed(TrainType elem){
    switch (elem){
    case TrainType::REGIONALE:
        return 160;
        break;
    case TrainType::ALTA_VELOCITA:
        return 240;
        break;
    case TrainType::ALTA_VELOCITA_SUPER:
        return 300;
        break;
    default:
        throw new Train::InvalidTrainType;
        break;
    }
}


#endif
