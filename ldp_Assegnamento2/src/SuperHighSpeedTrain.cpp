// @author Arjun Jassal, 1219611
#include "..\include\Train.h"
#include "..\include\TrainLine.h"

SuperHighSpeedTrain::SuperHighSpeedTrain(int ID, TrainDirection dir, TrainLine* l, TrainTime* time) 
        : Train(ID, TrainType::ALTA_VELOCITA_SUPER, dir, l, time){
            
    if(dir==TrainDirection::FORWARD){
        prevStation=line->get_station_list().getFirst();                        //prevStation -> prima stazione della linea
        nextStation=line->get_station_list().getFirst()->getNext();             //nextStation -> prossima stazione nella linea
        
        prevMainStation=prevStation;                                            //prevMainStation e prevStation coincidono
        nextMainStation=get_next_main_station(prevMainStation, *this);          //nextMainStation -> prossima stazione principale
    }
    else if(dir==TrainDirection::BACKWARD){
        distance=line->get_station_list().getLast()->getDistance();                          
        
        prevStation=line->get_station_list().getLast();                         //prevStation -> l'ultima stazione della linea
        nextStation=line->get_station_list().getLast()->getPrev();              //nextStation -> stazione precedente all'ultima
        
        prevMainStation=prevStation;                                            //prevMainStation e prevStation
        nextMainStation=get_next_main_station(prevMainStation, *this);          //nextMainStation -> stazione principale precedente all'ultima
    }
}

void SuperHighSpeedTrain::callTrain(StationSignal si){                         //chiamata alla stazione
    if(si==StationSignal::DEPARTURE_ALLOW){
		if(direction==TrainDirection::FORWARD && distance != 0)          
			nextStation = nextStation->getNext();
		else if (direction==TrainDirection::BACKWARD && distance != line->get_station_list().getLast()->getDistance())
			nextStation = nextStation->getPrev();                       //partenza dalla stazione

		if (fabs(prevStation->getDistance()-distance)<0.67) {
			currentSpeed=80;                                            //rallenta la velocità
			timer=-1;                                                   //timer di sosta resettato
			track=nullptr;                                              //binario dissociato
		}
		else {
			parking=false;
			currentSpeed=maxSpeed;
			prevStation=nextStation;
			track=nullptr;
			timer=-1;
		}
    }
	else                                                                //treno in arrivo
		parking=true;                                                   //treno si ferma in parcheggio
}

void SuperHighSpeedTrain::clock(int t){
    time=timeConversion(t);                                             //conversione tempo
    if(direction==TrainDirection::FORWARD)                 
        distance+=currentSpeed/60.0f;                                    //se il treno va avanti la distanza dall'origine aumenta
    else
        distance-=currentSpeed/60.0f;                                    //se il treno va indietro la distanza dall'origine diminuisce
    
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
	
    //treno si avvicina ad una stazione locale

    else if(!parking && fabs(nextStation->getDistance()-distance)<20 && track==nullptr && fabs(nextStation->getDistance()-distance)>15) 
		nextStation->eventIncomingTrain(this, TrainRequest::TRANSIT);
    
    //treno fermo in parcheggio si sposta in stazione
    if(!parking && fabs(nextMainStation->getDistance()-distance)<5 && timer==-1)
        currentSpeed=80;

    //treno arriva ai binari
    if(parking && fabs(nextMainStation->getDistance()-distance)<5 && currentSpeed!=0)
        currentSpeed=0;
    
    //treno si ferma al parcheggio
    if(parking && fabs(nextStation->getDistance()-distance)<5 && currentSpeed!=0)
        currentSpeed=0;

    //treno riparte dal parcheggio
    if(!parking && fabs(nextStation->getDistance()-distance)<5 && currentSpeed==0 && track==nullptr)
        currentSpeed=maxSpeed;

    //treno fermo in stazione
    if(timer==-1 && fabs(nextMainStation->getDistance()-distance)<0.67){
        currentSpeed=0;
        if((size_t)visitedStations+1==line->get_station_list().getIndexMain().size()-1){                //se arrivo al capolinea
                endline=true;
                track->update(TrackStatus::FREE);                                               //libero il binario
				if (getDelay()>0)
					std::cout << "\nIl treno " << trainID << " e' giunto al capolinea con " << getDelay() << " minuti di ritardo\n\n";
				else
					std::cout << "\nIl treno " << trainID << " e' giunto al capolinea in orario\n\n";
            }
        int delay=getDelay();
        if (delay > 0)
            std::cout << "\nIl treno " << trainID << " e' arrivato alla stazione " << nextStation->getLabel() << "con " << delay << " minuti di ritardo, l'orario attuale e\' " << t << "\n\n";
        else
            std::cout << "\nIl treno " << trainID << " e' arrivato alla stazione " << nextStation->getLabel() << "senza ritardo, l'orario attuale e\' " << t << "\n\n";
        timer=5;                            //parte il timer in cui il treno sta fermo in stazione
        if(delay<0)                         //se il treno è in anticipo
            timer+= -delay;    
        visitedStations++;                  //aumenta il numero di stazioni visitate
    }
    if(timer>0){
        timer--;
        if(timer==0){        
            nextMainStation->eventOutgoingTrain(this);                          //treno riparte
            prevMainStation=nextMainStation;                                    //la stazione principale attuale diventa la precedente
			prevStation=nextStation;                                            //la stazione locale attuale diventa la precedente
            if(direction==TrainDirection::FORWARD)          
                nextMainStation=get_next_main_station(prevMainStation,*this);           //il puntatore va alla prossima stazione principale
            else 
                nextMainStation=get_next_main_station(prevMainStation,*this);           //il puntatore va alla prossima stazione principale  
            
        }
    }
}
