// @author Arjun Jassal, 1219611
#include "generics.h"
#include "Station.h"
#include "TrainTime.h"
#include <string>

class Train{
    public:
        Train(int ID, int curr, int d, TrainType t, TrainDirection dir)
            : trainID{ID}, currentSpeed{curr}, delay{d}, TrainType{t}, TrainDirection{dir}{}
        ~Train();
        
        //GET
        int getTrainID() const; //{return trainID;}
        int getCurrentSpeed() const; //{return currentSpeed;}
        int getMaxSpeed() const; //{return maxSpeed;}
        int getDelay() const //{return delay;}
        TrainType getTrainType() const; //{return type;}
        TrainDirection getTrainDirection() const; //{return direction;}
        
        //SET
        void setCurrentSpeed(int speed); //{currentSpeed=speed;}
        void setDelay(int d);
        
        void callTrain(StationSignal si);
        void callTrain(StationSignal si, Track* track);

    private:
        const int trainID;
        int currentSpeed;
        const int maxSpeed;
        int delay=0;
        const TrainType type;
        TrainDirection direction;

}