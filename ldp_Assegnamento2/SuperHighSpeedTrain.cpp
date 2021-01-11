#include "Train.h"
#include "TrainLine.h"

SuperHighSpeedTrain::SuperHighSpeedTrain(int ID, TrainDirection dir, TrainLine* l, TrainTime* time) 
        : Train(ID, TrainType::ALTA_VELOCITA_SUPER, dir, l, time){
            
    if(dir==TrainDirection::FORWARD){
        prevStation=line->get_station_list().getFirst();                         //prevStation -> prima stazione della linea
        nextStation=line->get_station_list().getFirst()->getNext();              //nextStation -> prossima stazione nella linea
        
        prevMainStation=prevStation;                                             //prevMainStation e prevStation coincidono
        nextMainStation=get_next_main_station(prevMainStation, *this);           //nextMainStation -> prossima stazione principale
    }
    else if(dir==TrainDirection::BACKWARD){
        distance=line->get_station_list().getLast()->getDistance();                           
        
        prevStation=line->get_station_list().getLast();                          //prevStation -> l'ultima stazione della linea
        nextStation=line->get_station_list().getLast()->getPrev();               //nextStation -> stazione precedente all'ultima
        
        prevMainStation=prevStation;                                             //prevMainStation e prevStation
        nextStation=get_next_main_station(prevMainStation, *this);               //nextMainStation -> stazione principale precedente all'ultima
    }
}

void SuperHighSpeedTrain::clock(int t){
    time=timeConversion(t);                                             //conversione tempo
    if(direction==TrainDirection::FORWARD)                 
        distance+=currentSpeed/60.0;                                    //se il treno va avanti la distanza dall'origine aumenta
    else
        distance-=currentSpeed/60.0;                                    //se il treno va indietro la distanza dall'origine diminuisce
    
    int arrivalTime=getArrivalTime();

    //STAZIONI MAIN
    //treno parte dalla stazione origine
    if(startline && time==timeConversion(arrivalTime)){                 //se il treno è alla stazione iniziale e arriva il suo orario
        prevStation->eventOutgoingTrain(this);                          //treno parte
        startline=false;                                                //il treno non è più all'origine
    }

    //treno riprende la velocità dopo aver superato la stazione
    if(currentSpeed==80 && fabs(prevMainStation->getDistance()+distance)>5)
        currentSpeed=maxSpeed;

    //treno a 20km dalla stazione fa richiesta
    if(!parking && fabs(nextMainStation->getDistance()-distance)<20 && track==nullptr)
        nextMainStation->eventIncomingTrain(this, TrainRequest::STOP);
    
    //treno fermo in parcheggio si sposta in stazione
    if(!parking && fabs(nextMainStation->getDistance()-distance)<5 && timer==-1)
        currentSpeed=80;

    //treno arriva ai binari
    if(parking && fabs(nextMainStation->getDistance()-distance)<5 && currentSpeed!=0)
        currentSpeed=0;

    //treno si avvicina ad una stazione locale
    if(!parking && fabs(nextStation->getDistance()-distance)<20 && track==nullptr)
        nextStation->eventIncomingTrain(this, TrainRequest::TRANSIT);
    
    //treno si ferma al parcheggio
    if(parking && fabs(nextStation->getDistance()-distance)<5 && currentSpeed!=0)
        currentSpeed=0;

    //treno riparte dal parcheggio
    if(!parking && fabs(nextStation->getDistance()-distance)<5 && currentSpeed=0)
        currentSpeed=maxSpeed;

    //treno fermo in stazione
    if(timer==-1 && fabs(nextMainStation->getDistance()-distance)<0.67){
        currentSpeed=0;
        if(visitedStations+1==line->get_station_list().iterable().size()-1){                 //se arrivo al capolinea
                endline=true;
                track->update(TrackStatus::FREE);
                std::cout << "Il treno " << trainID << " e' giunto al capolinea con " << getDelay() << " minuti di ritardo\n";
            }
        int delay=getDelay();
        if(delay>0)
            std::cout << "Il treno è arrivato alla stazione " << nextMainStation->getLabel() << " con " << delay << " minuti di ritardo";
        timer=5;                          //parte il timer in cui il treno sta fermo in stazione
        if(delay<0)                       //se il treno è in anticipo
            timer+= -delay;    
        visitedStations++;
    }

    if(timer>0){
        timer--;
        if(timer=0){        
            nextMainStation->eventOutgoingTrain(this);                                  //treno riparte dalla stazione principale
            
            prevStation=nextStation;                                                    //stazione successiva diventa la precedente

            if(direction==TrainDirection::FORWARD){
                if(nextStation==nextMainStation){                                       //se la stazione attuale è principale
                    nextStation=nextStation->getNext();                                 //nextStation punta alla prossima stazione
                    nextMainStation=get_next_main_station(prevMainStation, *this);      //nextMainStation punta alla prossima stazione principale
                    prevMainStation=prevStation;                                        //prevMainStation invece coincide con prevStation;
                }
                else{                                                                   //se invece la stazione attuale non è principale
                    nextStation=nextStation->getNext();                                 //sposto solo nextStation avanti
                }
            }else{
                if(nextStation==nextMainStation){                                       //se la stazione attuale è principale                                           
                    nextStation=nextStation->getPrev();                                 //nextStation punta alla prossima(precedente in lista)
                    nextMainStation=get_next_main_station(prevMainStation, *this);      //nextMainStation punta alla prossima principale
                    prevMainStation=prevStation;                                        //prevMainStation coincide con prevStation;
                }
                else{                                                                   //se invece la stazione attuale non è principale
                    nextStation=nextStation->getPrev();                                 //sposto solo nextStation avanti                                 
                }
            }
        }
    }
}
