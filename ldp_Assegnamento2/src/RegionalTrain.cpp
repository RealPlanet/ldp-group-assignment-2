// @author Arjun Jassal, 1219611
#include "..\include\Train.h"
#include "..\include\TrainLine.h"

RegionalTrain::RegionalTrain(int ID, TrainDirection dir, TrainLine* l, TrainTime* time) 
        : Train(ID, TrainType::REGIONALE, dir, l, time){

    if(dir==TrainDirection::FORWARD){                                               
        prevStation=line->get_station_list().getFirst();                                //prec-> stazione iniziale
        nextStation=line->get_station_list().getFirst()->getNext();                     //next-> stazione iniziale+1
    }
    else if(dir==TrainDirection::BACKWARD){
        distance=line->get_station_list().getLast()->getDistance();

        prevStation=line->get_station_list().getLast();                                 //next-> stazione finale-1
        nextStation=line->get_station_list().getLast()->getPrev();                      //prec-> stazione finale
    }
}

void RegionalTrain::callTrain(StationSignal si) {                                       //chiamata alla stazione
    if(si==StationSignal::DEPARTURE_ALLOW){                                             //partenza dalla stazione
        currentSpeed=80;                                                                //rallenta la velocità
        timer=-1;                                                                       //timer di sosta resettato
		track=nullptr;                                                                  //binario dissociato
    }
	else                                                                                //treno in arrivo
		parking=true;                                                                   //treno si ferma in parcheggio
}

void RegionalTrain::clock(int t){
    time=timeConversion(t);                                         //conversione tempo

    if(direction==TrainDirection::FORWARD)                 
        distance+=currentSpeed/60.0f;                                //se il treno va avanti la distanza dall'origine aumenta
    else
        distance-=currentSpeed/60.0f;                                //se il treno va indietro la distanza dall'origine diminuisce
    
    int arrivalTime=getArrivalTime();

    if(startline && time==timeConversion(arrivalTime)){             //se il treno è alla stazione iniziale e arriva il suo orario
        prevStation->eventOutgoingTrain(this);                      //treno parte
        startline=false;                                            //il treno non è più all'origine
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
    if(timer==-1 && fabs(nextStation->getDistance()-distance)<0.67){
        currentSpeed=0;
        if((size_t)visitedStations+1==line->get_station_list().iterable().size()-1){                    //se arrivo al capolinea
                endline=true;
                track->update(TrackStatus::FREE);                                               //libero il binario
				if (getDelay()>0)
					std::cout << "\nIl treno " << trainID << " e' giunto al capolinea con " << getDelay() << " minuti di ritardo\n\n";
				else
					std::cout << "\nIl treno " << trainID << " e' giunto al capolinea in orario\n\n";
            }

        int delay=getDelay();
        if(delay>0)
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
            nextStation->eventOutgoingTrain(this);          //treno riparte
            prevStation=nextStation;                        //i due puntatori coincidono 
            if(direction==TrainDirection::FORWARD)          
                nextStation=nextStation->getNext();         //il puntatore va alla prossima stazione
            else
                nextStation=nextStation->getPrev();         //il puntatore va alla stazione precedente(la prossima)
            
        }
    }

}   