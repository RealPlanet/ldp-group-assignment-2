#include "Train.h"

class RegionalTrain : public Train{
    public:
    RegionalTrain(int ID, TrainType::REGIONALE, TrainDirection dir, TrainLine* l, TrainTime* time) 
        : public Train(ID, TrainType::REGIONALE, dir, l, time){}

    void clock(int t){
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
        if(!parking && fabs(nextStation->getDistance()-distance)<5 && timer=-1)
            currentSpeed=80;

        //treno arriva ai binari
        if(parking && fabs(nextStation->getDistance()-distance)<5 && currentSpeed!=0)
            currentSpeed=0;

        //treno fermo in stazione
        if(timer==-1 && fabs(nextStation->getDistance()-distance)<0){
            currentSpeed=0;
            if(visitedStation+1==line->get_station_list().iterator().size()-1){                 //se arrivo al capolinea
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
                    nextStation=nextStation->getNext();         //il puntatore va alla prossima stazione
                else
                    nextStation=nextStation->getPrev();
                
            }
        }

    }

}

/*
void clock(int t){

                /*
                    1. treno fa richiesta a 20km                    X
                    2. treno rallenta a 5km                         X
                    SE NON SI FERMA
                        3. treno prosegue 
                        4. superati i 5km riprende maxspeed
                    SE SI FERMA
                        SE VA NEL PARCHEGGIO (parking=true)
                            3. si ferma nel parcheggio
                            4. attende
                            5. si ferma al binario
                            6. scade il timer
                            7. treno prosegue
                            8. superati i 5km riprende maxspeed
                        SE NON VA NEL PARCHEGGIO (parking=false)
                            3. si ferma al binario
                            4. scade il timer
                            5. treno prosegue
                            6. superati i 5km riprende maxspeed
                */

            //treno fa richiesta a 20km
            if((direction==TrainDirection::FORWARD && nextStation.getDistance()-distance<20)
            || (direction==TrainDirection::BACKWARD && distance-nextStation.getDistance()<20)){
                nextStation->eventIncomingTrain(this, TrainRequest::STOP);
            }

            //treno si avvicina alla stazione(distanza<5km)
            if(currentSpeed==maxSpeed && (direction==TrainDirection::FORWARD && nextStation.getDistance()-distance<5 
            || direction==TrainDirection::BACKWARD && distance-nextStation.getDistance()<5)){
                currentSpeed=80;
            }

            //CASO TRENO NON SI FERMA ALLA STAZIONE
            //treno non si ferma -> prosegue
            if(currentSpeed==80 && ((direction==TrainDirection::FORWARD && prevStation.getDistance()+distance>5) 
            || (direction==TrainDirection::BACKWARD && prevStation.getDistance()-distance>5){
                currentSpeed=maxSpeed;
            }

            //CASO TRENO SI FERMA ALLA STAZIONE

            //treno si ferma in parcheggio
            if(parking && (direction==TrainDirection::FORWARD && nextStation.getDistance()-distance<5)
            || (direction==TrainDirection::BACKWARD && distance-nextStation.getDistance()<5)){
                currentSpeed=0; //treno fermo in parcheggio
            }

            //treno fermo in stazione
            if(timer==-1 &&
            (direction==TrainDirection::FORWARD && nextStation.getDistance()-distance<0)
            || (direction==TrainDirection::BACKWARD && distance-nextStation.getDistance()<0)){
                currentSpeed=0;
                //elimina il treno se è l'ultima stazione liberando il binario
                if(visitedStation+1==trainLine.m_station_list.iterator().size()){
                    endline=true;
                    track->update(TrackStatus::FREE);
                }

                delay=getDelay();
                if(delay>0) std::cout << "treno in ritardo di " << delay;



            }
        }
        */