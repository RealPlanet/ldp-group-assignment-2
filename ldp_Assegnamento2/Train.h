// @author Arjun Jassal, 1219611
#include "generics.h"
#include "Station.h"
#include "TrainTime.h"
#include <string>

static int getTrainSpeed(TrainType elem);

class Train{
    public:
        Train(int ID, TrainType t, TrainDirection dir. TrainLine& tl)
            : trainID{ID}, TrainType{t}, TrainDirection{dir}{
                maxSpeed=getTrainSpeed(t);
        }
        ~Train();
        
        //GET
        int getTrainID() const {return trainID;}
        int getCurrentSpeed() const {return currentSpeed;}
        int getMaxSpeed() const {return maxSpeed;}
        int getDelay() const {return delay;}
        TrainType getTrainType() const; {return type;}
        TrainDirection getTrainDirection() const; {return direction;}
        
        //SET
        void setCurrentSpeed(int speed); {currentSpeed=speed;}
        void setDelay(int d);
        
        Station* getNextStationToStop();
        int getNextStationTimeofArrival();
        void callTrain(StationSignal si);
        void callTrain(StationSignal si, Track* track);

        void clock();

        class InvalidTrainType:public std::exception{};

    protected:
        const int trainID;
        int currentSpeed=0;
        const int maxSpeed;
        int delay=0;
        const TrainType type;
        TrainDirection direction;
        std::map<int, TrainInfo>& timetable;
        Station* nextStation;
        Station* currentStation;
        int stationsCont=0;
        TrainLine& trainline;
        int currentTime=0;
        float distance;

}

---------------------------------------------------------------------------------------------------------------------------------

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
        throw new Train::InvalidTrainType();
        break;
    }
}


class Train(){
    public:
        Train(int ID, TrainType t, TrainDirection dir, TrainLine& l, TrainTime& t)
            : trainID{ID}, trainType{t}, direction{dir}{
                maxSpeed=getTrainSpeed(t);
                currentSpeed=0;
                distance=0;
                line=l;

                stazPrec=line.m_station_list.getFirst();
                stazNext=line.m_station_list.getFirst()->getNext();                             //modificare accesso di m_station_list   
                                                                                
                if(dir==TrainDirection::BACKWARD) {
                    line.m_station_list.getLast()->getDistance();                               //modificare accesso di m_station_list
                    stazNext=line.m_station_list.getLast()->getPrev();
                    stazPrec=line.m_station_list.getLast();
        }
        //GET
        TrainDirection getTrainDirection() {return direction;}
        int getMaxSpeed() {return maxSpeed;}
        int getTrainID() {return trainID;}
        
        void callTrain(StationSignal si){
            //arrivo stazione
            if(si==StationSignal::DEPARTURE_ALLOW){
                currentSpeed=80;
            }else{
                parking=true;
            }
        }

        void callTrain(StationSignal si, Track* t){
            parking=false;
            track=t;
        }

        int getDelay(){
            int t=trainTime.get_train_info(trainID).m_train_times.at(visitedStations+1);
            return time-(t/100*60+t%100);
        }

        int getTimeArrivalNextStation(){
            return trainTime.get_train_info(trainID).m_train_times.at(visitedStations+1);
        }

        void clock(int t){
            time=t/100*60+t%100;
            if(currentSpeed==80 && 
            ((direction==TrainDirection::FORWARD && stazPrec.getDistance()+distance>5) 
            || (direction==TrainDirection::BACKWARD && stazPrec.getDistance()-distanze>5){
                currentSpeed=maxSpeed;
            }

            if((direction==TrainDirection::FORWARD && stazNext.getDistance()-distance<20)
            || (direction==TrainDirection::BACKWARD && distance-stazNext.getDistance()<20)){
                stazNext->eventIncomingTrain(this, TrainRequest::STOP);
            }

            if(parking && currentSpeed!=80 &&
            (direction==TrainDirection::FORWARD && stazNext.getDistance()-distance<5)
            || (direction==TrainDirection::BACKWARD && distance-stazNext.getDistance()<5)){
                currentSpeed=80;
            }

            if(parking&&
            (direction==TrainDirection::FORWARD && stazNext.getDistance()-distance<5)
            || (direction==TrainDirection::BACKWARD && distance-stazNext.getDistance()<5)){
                currentSpeed=0;
            }

            if(timer==-1 &&
            (direction==TrainDirection::FORWARD && stazNext.getDistance()-distance<0)
            || (direction==TrainDirection::BACKWARD && distance-stazNext.getDistance()<0)){
                currentSpeed=0;
                if(visitedStation+1==trainLine.m_station_list.iterator().size()){
                    endline=true;
                    track->update(TrackStatus::FREE);
                }
                delay=getDelay();
                if(delay>0) std::cout << "treno in ritardo di " << delay;

                timer=5;
                if(delay<0){
                    time+=delay;
                }
                visitedStations++;
            }

            if(time>0){
                timer--;
                if(timer==0){
                    stazNext->eventOutgoingTrain(this);
                    stazPrec=stazNext;
                    if(TrainDirection::FORWARD){
                        stazNext=stazNet->getNext();
                    }else{
                        stazNext=stazNext->getPrev();
                    }
                }
            }



        }

        bool getEndList(){return endline};


    private:
        int trainID;
        TrainType trainType;
        TrainDirection direction;

        int timer=-1;

        bool endline=false;
        bool parking=false;

        int visitedStations;

        Track* track;

        TrainLine& line;
        TrainTime& trainTime;

        int maxSpeed;
        int currentSpeed;

        int time;

        int distance;
        Station* stazPrec;
        Station* stazNext;
    }
    

}