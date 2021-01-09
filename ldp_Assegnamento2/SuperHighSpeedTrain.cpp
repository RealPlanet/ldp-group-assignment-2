#include "generics.h"

class HighSpeedTrain : public Train{
    public:
        HighSpeedTrain(int ID, TrainType::ALTA_VELOCITA_SUPER, TrainDirection dir, TrainLine* l, TrainTime* time) 
        : public Train(ID, TrainType::ALTA_VELOCITA_SUPER, dir, l, time){}

    void clock(int t){}
}