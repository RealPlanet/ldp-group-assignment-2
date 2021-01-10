#include "Train.h"
#include "TrainLine.h"

SuperHighSpeedTrain::SuperHighSpeedTrain(int ID, TrainDirection dir, TrainLine* l, TrainTime* time) 
        : Train(ID, TrainType::ALTA_VELOCITA_SUPER, dir, l, time){
            
    if(dir==TrainDirection::FORWARD){
        prevStation=line->get_station_list().getFirst();                                //prec-> stazione iniziale
        nextStation=get_next_main_station(prevStation, *this);                          //next-> stazione iniziale+1
    }else if(dir==TrainDirection::BACKWARD){
        distance=line->get_main_station_list().getLast()->getDistance();                           
        
        prevStation=line->get_station_list().getLast();                                 //next-> stazione principale finale-1
        nextStation=get_next_main_station(prevStation, *this);                          //prec-> stazione finale
    }
}

void SuperHighSpeedTrain::clock(int t){
    time=timeConversion(t);                                     //conversione tempo
    if(direction==TrainDirection::FORWARD)                 
        distance+=currentSpeed/60.0;                            //se il treno va avanti la distanza dall'origine aumenta
    else
        distance-=currentSpeed/60.0;                            //se il treno va indietro la distanza dall'origine diminuisce
    
    int arrivalTime=getArrivalTime();
    if(startline && time==timeConversion(t)){                   //se il treno è alla stazione iniziale e arriva il suo orario
        prevStation->eventOutgoingTrain(this);                  //treno parte
        startline=false;                                        //il treno non è più all'origine
    }
    //treno riprende la velocità dopo aver superato la stazione
    if(currentSpeed==80 && fabs(prevStation->getDistance()+distance)>5)
        currentSpeed=maxSpeed;
    //treno a 20km dalla stazione fa richiesta
    if(!parking && fabs(nextStation->getDistance()-distance)<20 && track==nullptr)
            nextStation->eventIncomingTrain(this, TrainRequest::STOP);
    //treno fermo in parcheggio si sposta in stazione
    if(!parking && fabs(nextStation->getDistance()-distance)<5 && timer==-1)
        currentSpeed=80;
    //treno arriva ai binari
    if(parking && fabs(nextStation->getDistance()-distance)<5 && currentSpeed!=0)
        currentSpeed=0;
    //treno fermo in stazione
    if(timer==-1 && fabs(nextStation->getDistance()-distance)<0){
        currentSpeed=0;
        if(visitedStations+1==line->get_station_list().iterable().size()-1){                 //se arrivo al capolinea
                endline=true;
                track->update(TrackStatus::FREE);
                std::cout << "Il treno " << trainID << " e' giunto al capolinea con " << getDelay() << " minuti di ritardo\n";
            }
        int delay=getDelay();
        if(delay>0)
            std::cout << "Il treno è arrivato alla stazione " << nextStation->getLabel() << "con " << delay << " minuti di ritardo";
        timer=5;                          //parte il timer in cui il treno sta fermo in stazione
        if(delay<0)                       //se il treno è in anticipo
            timer+= -delay;    
        visitedStations++;
    }
    if(timer>0){
        timer--;
        if(timer=0){        
            nextStation->eventOutgoingTrain(this);          //treno riparte
            prevStation=nextStation;                        //i due puntatori coincidono 
            if(direction==TrainDirection::FORWARD)          
                nextStation=get_next_main_station(prevStation,*this);         //il puntatore va alla prossima stazione
            else
                nextStation=get_next_main_station(prevStation,*this);
            
        }
    }
}
