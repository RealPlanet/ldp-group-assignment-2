// @author Arjun Jassal, 1219611
#include "generics.h"
#include "Station.h"
#include "TrainTime.h"

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

static int timeConversion(int t){
    return (t/100*60+t%100);
}

class Train(){
    public:
        Train(int ID, TrainType t, TrainDirection dir, TrainLine* l, TrainTime* time)
            : trainID{ID}, trainType{t}, direction{dir}, line{l}, trainTime{time} {
                maxSpeed=getTrainSpeed(t);
                currentSpeed=0;
                distance=0;

                if(t==TrainType::REGIONALE){
                    if(dir==TrainDirection::FORWARD){                                               
                        prevStation=line->m_station_list.getFirst();                                //prec-> stazione iniziale
                        nextStation=line->m_station_list.getFirst()->getNext();                     //next-> stazione iniziale+1
                    }
                    else if(dir==TrainDirection::BACKWARD){
                        distance=line->m_station_list.getLast()->getDistance();                            
                        nextStation=line->m_station_list.getLast()->getPrev();                      //prec-> stazione finale
                        prevStation=line->m_station_list.getLast();                                 //next-> stazione finale-1
                    }
                }else{
                    if(dir==TrainDirection::FORWARD){
                    prevStation=line->get_main_station().getFirst();                                //prec-> stazione iniziale
                    nextStation=line->get_main_station().getFirst()->getNext();                     //next-> stazione principale successiva
                    }
                    else if(dir==TrainDirection::BACKWARD){
                    distance=line->get_main_station().getLast()->getDistance();                           
                    nextStation=line->get_main_station().getLast()->getPrev();                       //prec-> stazione finale
                    prevStation=line->get_main_station().getLast();                                  //next-> stazione principale finale-1
                    }
                }
        }

        //GET
        TrainDirection getTrainDirection() {return direction;}
        TrainType getTrainType() {return trainType;}
        int getMaxSpeed() {return maxSpeed;}
        int getTrainID() {return trainID;}
        float getDistance() {return distance;}
        bool isEndline() {return endline;}
        
        //COMUNICAZIONE CON STAZINE
        void callTrain(StationSignal si){                                                       //chiamata alla stazione
            if(si==StationSignal::DEPARTURE_ALLOW){                                             //partenza dalla stazione
                currentSpeed=80;                                                                //rallenta la velocità
                timer=-1;                                                                       //timer di sosta resettato
                track=nullptr;                                                                  //binario dissociato
            }
            else                                                                               //treno in arrivo
                parking=true;                                                                   //treno si ferma in parcheggio
        }

        void callTrain(StationSignal si, Track* t){                                             //chiamata alla stazione con binario
            parking=false;                                                                      //treno non si ferma in parcheggio
            track=t;                                                                            //assegna il binario
        }

        //CLOCK TRENO
        virtual void clock(int t);

        //RITARDO TRENI
        int getDelay(){
            int t=trainTime->get_train_info(trainID).m_train_times.at(visitedStations+1);        //differenza tra orario corrente e previsto
            return time-timeConversion;
        }

        //ORARIO DI ARRIVO PREVISTO ALLA STAZIONE SUCCESSIVA
        int getNextArrivalTime(){
            return trainTime->get_train_info(trainID).m_train_times.at(visitedStations+1);       //ritorna l'orario previsto alla stazione+1(successiva)
        }

        int getArrivalTime(){
            return trainTime->get_train_info(trainID).m_train_times.at(visitedStations);
        }

    
    private:
        int trainID;                                                //ID del treno
        TrainType trainType;                                        //tipo del treno
        TrainDirection direction;                                   //direzione del treno

        int timer=-1;                                               //timer di sosta in stazione

        bool startline=true;                                        //controllo se è la prima stazione della linea
        bool endline=false;                                         //controllo se è l'ultima stazione della linea
        
        bool parking=false;                                         //controllo se va in parcheggio o no

        int visitedStations;                                        //contatore delle stazioni visitate

        Track* track;                                               //binario associato in arrivo alla stazione

        TrainLine* line;                                            //linea associata al treno
        TrainTime* trainTime;                                       //tabella degli orari associata al treno

        int maxSpeed;                                               //velocità massima
        int currentSpeed;                                           //velocità di crociera

        int time;                                                   

        float distance;                                             //distanza totale percorsa da 0
        Station* prevStation;                                          //stazione precedente
        Station* nextStation;                                          //stazione successiva
    }
}