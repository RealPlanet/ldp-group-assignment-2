// @author Matteo Salvalaio, 1216361

#ifndef STATION_H
#define STATION_H

#include <string>
#include <iostream>
#include "generics.h"
#include "TrainPQ.h"

class Track;
class Train;

/************************************************
 *  Lo struct contiene al suo interno le liste di attesa
 *  dei treni alle stazioni, e l'elenco e il tipo di
 *  binari per ogni senso di marcia
 ***********************************************/
 
//Invariants
// - lastTime >= -1
// - nextTime >= -1

struct LineWay {
	explicit LineWay(Station* currentStation)   //Default Constructor
	: lastTime{-1}, lastTrain{nullptr}, departures(currentStation) {}
	
	Train* lastTrain;                           //Last train to depart
	int lastTime;                               //Last train departure time
	int nextTime = 0;                           //Next train departure time forward
	
	ArrivalsTrainPQ arrivals;                   //Priority Queue of arrivals
	DeparturesTrainPQ departures;               //Priority Queue of departures
	std::vector<Track*> tracks;                 //List of tracks of the station
};

//Invariants
// - distance >= 0
// - stationSpeedLimit > 0
// - aheadTolerace >= 0
// - stationType must be initialized

/************************************************
 *  Interfaccia comune delle stazioni.
 *  La classe gestisce se stessa come un nodo di una lista
 *  Dato che le stazioni locali e principali differiscono
 *  di poco tra di loro, la maggior parte dei metodi,
 *  comuni a tutte e due le classi derivate sono definiti
 *  qui.
 *  Il costruttore di copia e l'assegnamento di copia
 *  sono stati disabilitati in quanto non è necessario
 *  creare una copia della classe station in questo progetto.
 ***********************************************/

class Station {
public:
									//*** Essential Operations ***
	
	explicit Station(float d = 0, std::string l = "Unnamed", StationType sType = StationType::MAIN)   //Default Constructor
	: distance{d}, label{l}, next{nullptr}, prev{nullptr}, stationType{sType} {
		forward = new LineWay(this);
		backward = new LineWay(this);
		}
	
	~Station();                                                                     //Destructor
	
	Station(const Station& station) = delete;                                       //Copy Constructor
	Station& operator=(const Station& station) = delete;                            //Copy Assignment
	Station(Station&& station) = default;                                           //Move Constructor
	Station& operator=(Station&& station) = default;                                //Move Assignment
	
									//*** Node Modification/Inspection ***
	
	void setNext(Station* station);                                                 //Set the next station
	void setPrev(Station* station);                                                 //Set the previous station
	
	Station* getNext() const;                                                       //Return the next station, if it exist
	Station* getPrev() const;                                                       //Return the previous station, if it exist
	
	bool hasNext() const {return next!=nullptr;}                                    //Check if this station has a subsequent station
	bool hasPrev() const {return prev!=nullptr;}                                    //Check if this station has a precedent station
	bool isFirst(Station* station) const {return prev==nullptr;}                    //Check if the station is the first
	bool isLast(Station* station) const {return next==nullptr;}                     //Check if the station is the last
	
									//*** Function Operations ***
	
	float getDistance() const {return distance;}                                    //Return the distance of the station from the origin
	std::string getLabel() const {return label;}                                    //Return the name of the station
	StationType getStationType() const {return stationType;}                        //Return the station type
	
	virtual void eventIncomingTrain(Train* train, const TrainRequest request) = 0;  //Receives the call from the train arriving at the station
	void eventOutgoingTrain(Train* train);                                          //Receives the call from the train that is ready to leave the station
	
	void clock(int time);                                                           //Check arrivals and departures every minute of the simulation
									//*** Exception Management ***
	
	class OutOfBoundsException : public std::exception {};                          //Exception of the class Station
	class FreeTrackNotFoundException : public std::exception {};                    //Exception of the class Station
	class ImpossibleDeparturesException : public std::exception {};                 //Exception of the class Station
	class InvalidFunctionCallException : public std::exception {};                  //Exception of the class Station
	class InvalidTimeException : public std::exception {};                          //Exception of the class Station
	
protected:
	
	Station* next;                                                                  //Next station in the line
	Station* prev;                                                                  //Previous station in the line
	
	const StationType stationType;                                                  //Type of station
	
	const float distance;                                                           //Distance from origin
	const std::string label;                                                        //Name of the station
	
	const int stationSpeedLimit = 80;                                               //Speed limit 10km around the station
	const int aheadTolerace = 15;                                                   //Limit of tolerance for early trains
	
	LineWay* forward;                                                               //Priority queues and tracks for train coming from the first station
	LineWay* backward;                                                              //Priority queues and tracks for train going to the first station
	
	void callArrivals(LineWay* way);                                                //Manages and allow trains to arrive at the standard track of the station
	virtual void callDepartures(LineWay* way) = 0;                                  //Manages and allow trains to depart from the station
	void eventDepartedTrain(int time, LineWay* way);                                //Tell the train to depart from the station and update the departure queue
	
	Track* getTrack(TrackType type, LineWay* way) const;                            //Return the first free track;
	bool isTrackFree(TrackType type, LineWay* way) const;                           //Check is there is at least one track free
	
};

/************************************************
 *  Classi derivate di Station.
 ***********************************************/

class MainStation : public Station {
public:
	explicit MainStation(float d = 0, std::string l = "Unnamed");                      //Default Constructor
	void eventIncomingTrain(Train* train, TrainRequest request) override;            //Implementation of eventIncomingTrain from Station
	
	class InvalidRequestException : public std::exception {};                        //Exception of the class MainStation
	
private:
	void callDepartures(LineWay* way) override;                                     //Implementation of callDepartures from Station
};

class LocalStation : public Station {
public:
	explicit LocalStation(float d = 0, std::string l = "Unnamed");                    //Default Constructor
	void eventIncomingTrain(Train* train, TrainRequest request) override;           //Implementation of eventIncomingTrain from Station
	
private:
	void callDepartures(LineWay* way) override;                                     //Implementation of callDepartures from Station
};

#endif

/************************************************
 *  Informazioni e gestione delle partenze dei treni dalle stazioni
 * 
 *  La stazione calcolerà automaticamente il tempo necessario affinchè 
 *  il treno successivo (A) arrivi a 15km dalla stazione successiva,
 *  quando il treno precedente (B) è ad esattamente 5km dalla stazione successiva
 *  (limite dei 10km di distanza, esente negli ultimi 5km)
 *  in modo che il treno successivo (A) possa viaggiare sempre alla velocità massima consentita
 * 
 *  Tramite questo sistema, i ritardi dei treni non avverrano mai a causa di rallentamenti lungo
 *  la tratta, ma a causa di una partenza ritardata.
 *  
 *  La presenza di un numero maggiore di treni di quanto la stazione possa gestire / treni eccessivamente in
 *  anticipo (per non occupare eccessivamente un binario) vengono indirizzati al parcheggio dove vengono messi
 *  in lista di attesa in base alla priorità e al loro ritardo/anticipo attuale per poter andare in stazione.
 *  Il programma punta a prioritizzare la presenza di numerosi lievi ritardi, al posto di singoli grandi ritardi.
 * 
 *  I treni in stazione in attesa di partire e i treni al parcheggio in attesa di transitare condividono la stessa
 *  lista di attesa.
 *  
 *                                             un treno è in arrivo
 *                                                     |
 *                                                     |
 *                           Transito ---------- cosa richiede? ----------Fermata
 *                              |                                            |
 *                              |                                            |
 *                     mandato al parcheggio                         i binari sono occupati?
 *                aggiunto alla lista per partire          NO ------ il treno è in anticipo? ----- SI
 *                              |                          |                                        |
 *                      arriva al parcheggio        mandato al binario    <-------         mandato al parcheggio
 *                              |                          |                     |                  |
 *                              |                          |                     |                  |
 *                              |                     dopo 5 minuti              |     il treno riceve un binario al
 *                              |             aggiunto alla lista per partire    ------        quale recarsi
 *                              |                          |
 *                              |                          |
 *                       il treno riceve l'autorizzazione a partire
 * 
 * 
 *  Nel caso ideale i treni ricevono l'autorizzazione a partire / il binario al quale recarsi prima ancora di
 *  arrivare al parcheggio. In tal caso ignorano l'ordine precedente e partono il prima possibile.
 *  
 *  C'è la possibilità che mentre un treno (A) è in arrivo a ~5-10km alla stazione
 *  nello stesso momento ad un treno (B) fermo al parcheggio sia data la disponibilità di andare
 *  al binario di transito per continuare la sua corsa. In questo caso i due treni per un breve periodo
 *  saranno a distanza minore di 10km su una tratta senza limite di velocità.
 * 
 *                <------ < 10km ------>
 *  -----Treno (A)------------|---------Treno(B)---------|========|
 *                            |                           Stazione
 *                       Parcheggio
 * 
 *  Si è deciso di ignorare questo problema in quanto quando questa eventualità accade
 *  il treno (A) ha già sicuramente ricevuto ordine di andare al parcheggio/andare ad un binario standard.
 *  Quindi i due treni non si incontreranno mai, in quanto i loro percorsi seguono tratte separate.
 * 
 * 
 * 
 * 
 *  Tabella dei tempi di attesa in partenza ipotetici tra due stazioni [A] -----> [B] distanti 50 Km
 * 
 *  Tempi di percorrenza complessiva [Fermata] -----> [5km dalla stazione successiva]
 *      Treno Regionale ---> 5/80 + X-10/160 --> 3.75 + 15 = 18:45 minuti
 *      Treno AltaVeloc ---> 5/80 + X-10/240 --> 3.75 + 10 = 13:45 minuti
 *      Treno Altissima ---> 5/80 + X-10/300 --> 3.75 + 8  = 11:45 minuti
 *  Tempi di percorrenza complessiva [Transito] -----> [5km dalla stazione successiva]
 *      Treno Regionale ---> X-5/160 --> 16.875 = 16:52.5 minuti		(Questo caso non dovrebbe verificarsi mai)
 *      Treno AltaVeloc ---> X-5/160 --> 11.25  = 11:15   minuti
 *      Treno Altissima ---> X-5/160 --> 9      =  9:00   minuti
 * 
 *  Appena partito un [Treno Regionale] i tempi di attesa di treni in [Fermata] sono
 *      Treno Regionale ---> 5/80 + 5/160 ---> 3.75 + 1.875 = Parte dopo 5:27.5 minuti                      (non guadagna spazio nel tempo)
 *      Treno AltaVeloc ---> (per 40km) 5/80 + X-20/240 ---> 3.75 + 7.5 = 11:15 minuti ------> Parte dopo 7:30 dal Regionale
 *      Treno Altissima ---> (per 40km) 5/80 + X-20/300 ---> 3.75 + 6   =  9:45 minuti ------> Parte dopo 9:00 dal Regionale
 *  Appena partito un [Treno Alta Velocità] i tempi di attesa di treni in [Fermata] sono
 *      Treno Regionale/AltaVeloc ---> 5/80 +  5/240 ---> 3.75 + 1.25 = Parte dopo 5:00 minuti dal AltaVeloc (non guadagna spazio nel tempo)
 *      Treno Altissima           ---> 5/80 + X-20/300 ---> 3.75 + 6    = 9:45 minuti ------> Parte dopo 7:00 dal Altaveloc
 *  Appena partito un [Treno Alta Velocità Super] i tempi di attesa di treni in [Fermata] sono
 *      Treno Regionale/AltaVeloc/Altissima ---> 5/80 + 5/300 ---> 3.75 + 1  = Parte dopo 4:45 minuti dal Altissimo (non guadagna spazio nel tempo)
 * 
 * I conteggi differiscono se i treni devono transitare, data la mancanza del limite di velocità e la loro attesa al parcheggio
 * 
 ************************************************/ 
