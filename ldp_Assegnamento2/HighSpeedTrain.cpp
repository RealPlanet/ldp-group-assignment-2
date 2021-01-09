#include "generics.h"

class HighSpeedTrain : public Train{
    public:
        HighSpeedTrain(int ID, TrainType::ALTA_VELOCITA, TrainDirection dir, TrainLine* l, TrainTime* time) 
        : public Train(ID, TrainType::ALTA_VELOCITA, dir, l, time){}

    void clock(int t){}
}