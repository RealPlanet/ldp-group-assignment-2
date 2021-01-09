#include "generics.h"

class RegionalTrain : public Train{
    public:
    RegionalTrain(int ID, TrainType::REGIONALE, TrainDirection dir, TrainLine* l, TrainTime* time) 
        : public Train(ID, TrainType::REGIONALE, dir, l, time){}

    void clock(int t){
        
    }
}












void clock(int t){
            time=t/100*60+t%100;
            /*
            //superamento dei 5km dalla stazione -> treno massima velocità
            if(currentSpeed==80 &&
            ((direction==TrainDirection::FORWARD && stazPrec.getDistance()+distance>5) 
            || (direction==TrainDirection::BACKWARD && stazPrec.getDistance()-distance>5){
                currentSpeed=maxSpeed;
            }

            //richiesta treno in arrivo
            if((direction==TrainDirection::FORWARD && stazNext.getDistance()-distance<20)
            || (direction==TrainDirection::BACKWARD && distance-stazNext.getDistance()<20)){
                stazNext->eventIncomingTrain(this, TrainRequest::STOP);
            }
    
            if(parking && currentSpeed!=80 &&
            (direction==TrainDirection::FORWARD && stazNext.getDistance()-distance<5)
            || (direction==TrainDirection::BACKWARD && distance-stazNext.getDistance()<5)){
              currentSpeed=80;
            }
            //treno in parcheggio
            if(parking &&
            (direction==TrainDirection::FORWARD && stazNext.getDistance()-distance<5)
            || (direction==TrainDirection::BACKWARD && distance-stazNext.getDistance()<5)){
                currentSpeed=0;
            }

            //treno fermo in stazione
            if(timer==-1 &&
            (direction==TrainDirection::FORWARD && stazNext.getDistance()-distance<0)
            || (direction==TrainDirection::BACKWARD && distance-stazNext.getDistance()<0)){
              currentSpeed=0;
              //elimina il treno se è l'ultima stazione liberando il binario
                if(visitedStation+1==trainLine.m_station_list.iterator().size()){
                    endline=true;
                    track->update(TrackStatus::FREE);
                }
                delay=getDelay();
                if(delay>0) std::cout << "treno in ritardo di " << delay;
            
            
                timer=5;
                visitedStations++;
            }*/

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
            if((direction==TrainDirection::FORWARD && stazNext.getDistance()-distance<20)
            || (direction==TrainDirection::BACKWARD && distance-stazNext.getDistance()<20)){
                stazNext->eventIncomingTrain(this, TrainRequest::STOP);
            }

            //treno si avvicina alla stazione(distanza<5km)
            if(currentSpeed==maxSpeed && (direction==TrainDirection::FORWARD && stazNext.getDistance()-distance<5 
            || direction==TrainDirection::BACKWARD && distance-stazNext.getDistance()<5)){
                currentSpeed=80;
            }

            //CASO TRENO NON SI FERMA ALLA STAZIONE
            //treno non si ferma -> prosegue
            if(currentSpeed==80 && ((direction==TrainDirection::FORWARD && stazPrec.getDistance()+distance>5) 
            || (direction==TrainDirection::BACKWARD && stazPrec.getDistance()-distance>5){
                currentSpeed=maxSpeed;
            }

            //CASO TRENO SI FERMA ALLA STAZIONE

            //treno si ferma in parcheggio
            if(parking && (direction==TrainDirection::FORWARD && stazNext.getDistance()-distance<5)
            || (direction==TrainDirection::BACKWARD && distance-stazNext.getDistance()<5)){
                currentSpeed=0; //treno fermo in parcheggio
            }

            //treno fermo in stazione
            if(timer==-1 &&
            (direction==TrainDirection::FORWARD && stazNext.getDistance()-distance<0)
            || (direction==TrainDirection::BACKWARD && distance-stazNext.getDistance()<0)){
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