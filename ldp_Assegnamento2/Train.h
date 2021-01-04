// @author Arjun Jassal, 1219611
#include "generics.h"
#include "Station.h"
#include "TrainTime.h"
#include <string>

static int getTrainSpeed(TrainType elem);

class Train{
    public:
        Train(int ID, TrainType t, TrainDirection dir)
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
        void setNextStation(Station* next);
        
        void callTrain(StationSignal si);
        void callTrain(StationSignal si, Track* track);

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

}

class RegionalTrain : public Train{
    public:
        RegionalTrain()
}

static int getTrainSpeed(TrainType elem){
    switch (elem){
    case TrainType::REGIONALE:
        return 160;
        break;
    case TrainType::ALTA_VELOCITA:
        return 240;
        break;
    case TrainType::ALTA_VELOCITA:
        return 300;
        break;
    default:
        throw new Train::InvalidTrainType();
        break;
    }
}
