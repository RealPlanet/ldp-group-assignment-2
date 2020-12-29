// @author Matteo Salvalaio, 1216361

#ifndef STATION_H
#define STATION_H

#include <queue>
#include <string>

//Invariants
// - distance >= 0
// - stationSpeedLimit > 0
// - stationType = StationType.MAIN || StationType.LOCAL

struct LineWay {
	std::priority_queue<Train, std::vector<Train>, std::greater<int>> arrivals;			//Priority Queue of arrivals
	std::priority_queue<Train, std::vector<Train>, std::greater<int>> transits;			//Priority Queue of transits
	std::priority_queue<Train, std::vector<Train>, std::greater<int>> departures;		//Priority Queue of departures
	std::vector<Track> tracks;															//List of tracks of the station
}

class Station {
public:
	explicit Station(int d = 0, std::string l = "Unnamed", int s = 80)		//Default Constructor
	: distance{d}, label{l}, stationSpeedLimit{s} {}
	
	~Station() = default;													//Distruttore
	
	Station(const Station& node) = delete;									//Copy Constructor
	Station& operator=(const Station& node) = delete;						//Copy Assignment
	Station(Station&& node) = default;										//Move Constructor
	Station& operator=(Station&& node) = default;							//Move Assignment
	
	getDistance() const {return distance;}									//Return the distance of the station from the origin
	std::string getLabel() const {return label;}							//Return the name of the station
	
	virtual void incomingTrain(Train train, TrainRequest request) = 0;		//Receives the call from the train arriving at the station
	virtual void outgoingTrain(Train train) = 0;							//Receives the call from the train that is leaving the station
	void readyTrain(Train train);											//Receives the call from the train that is ready to leave the station
	
	void clearDeparturesConflict(LineWay& way, Train train);				//Prevents priority changes from causing conflicts between departures
	
protected:
	const int distance;														//Distance from origin
	const std::string label;												//Name of the station
	
	int lastTime;															//Last train departure time
	int lastTrain;															//Last train to depart
	
	const int stationSpeedLimit;											//Speed limit 10km around the station
	
	LineWay forward;														//Priority queues and tracks for train coming from the first station
	LineWay backward;														//Priority queues and tracks for train going to the first station
	
	void callArrivals(LineWay& way);										//Manages and allow trains to arrive at the standard track of the station
	void callDepartures(LineWay& way);										//Manages and allow trains to depart from the station

	Track getTrack(TrackType type, LineWay& way);							//Return the first free track;
	bool isTrackFree(TrackType type, LineWay& way);							//Check is there is at least one track free
};

class MainStation : public Station {
public:
	explicit Station(int d = 0, std::string l = "Unnamed", int s = 80)				//Default Constructor
	: distance{d}, label{l}, stationSpeedLimit{s} {
		forward.tracks.push_back(Track(STANDARD));
		forward.tracks.push_back(Track(STANDARD));
		backward.tracks.push_back(Track(STANDARD));
		backward.tracks.push_back(Track(STANDARD));
	}
	
	void incomingTrain(Train train, TrainRequest request);							//Implementation of incomingTrain from Station
	void outgoingTrain(Train train);												//Implementation of outcomingTrain from Station
};

class LocalStation : public Station {
public:
	explicit Station(int d = 0, std::string l = "Unnamed", int s = 80)				//Default Constructor
	: distance{d}, label{l}, stationSpeedLimit{s} {
		forward.tracks.push_back(Track(STANDARD));
		forward.tracks.push_back(Track(STANDARD));
		backward.tracks.push_back(Track(STANDARD));
		backward.tracks.push_back(Track(STANDARD));
		forward.tracks.push_back(Track(TRANSIT));
		backward.tracks.push_back(Track(TRANSIT));
	}
	
	void callTransits(LineWay& way);												//Manages and allow trains to arrive at the transit track of the station
	
	void incomingTrain(Train train, TrainRequest request);							//Implementation of incomingTrain from Station
	void outgoingTrain(Train train);												//Implementation of outcomingTrain from Station
};

#endif

//attenzione nel conto del tempo. i treni sul binario transit non hanno il vincolo dei 80km/h


/*
void readyTrain(Train train) {
	LineWay& way;
	if (train.getTrainDirection() == FORWARD) way = forward;
	else way = backward;
	way.departures.push(train);
	callDepartures(way);
}



	
	callArrivals(LineWay& way) {
		if (way.arrivals.size() != 0) {
			if (isTrackFree(STANDARD, way) && way.arrivals.top.getEstimatedTimeofArrival() - Runtime.getCurrentTime() < 15) {
				Track track getTrack(STANDARD, way);
				callTrain(track);
				track.setTrackStatus(OCCUPIED);
				way.arrivals.pop();
			}
		}
	
	void incomingTrain(Train train, TrainRequest request) {
		LineWay& way;
		if (train.getTrainDirection() == FORWARD) way = forward;
		else way = backward;
		if (request == STOP) {
			if (isTrackFree(STANDARD, way) && (train.getEstimatedTimeofArrival() - Runtime.getCurrentTime() < 15){
				Track track getTrack(STANDARD, way);
				callTrain(track);
				track.setTrackStatus(OCCUPIED);
				return;
			}
			else {
				way.arrivals.push(train);
				callTrain(getTrack(PARKING, way));//come fa planet il parking????
				return;
			}
		}
		throw std::invalid_argument("invalid status :( ");//invalid status
	}
	
	
	noConflictAdd(LineWay& way, Train train) {
		if (way.departures.size() != 0) {
			Train oldFirst = way.departures.top();
			way.departures.push(train);
			if (train == way.departures.top()) {
				oldFirst.callTrain("STA FERMO! non parti più!");
				callDepartures(LineWay& way);
			}
		}
		way.departures.push(train);
		callDepartures(LineWay& way);
	}
	
	callDepartures(LineWay& way) {
		if (way.departures.size() != 0) {
			

			int a = lastDeparturesTrain.getMaxSpeed()*(abs(getNextStation.getDistance-distance)-10) + 5*stationSpeedLimit;
			int b = way.departures.top().getMaxSpeed()*(abs(getNextStation.getDistance-distance)-20) + 5*stationSpeedLimit;
			int c = a-b
			way.departures.top().callTrain("Parti a" lastDeparturesTime+c) //ovvio che se actual time > di sta roba parti comunque!
			}
	}
	
	//DA FAR ACNORA
	//il treno ti segnala la partenza!!!! il treno ti segnala che è pronto!!!
	
	void outcomingTrain(Train train) {
		lastDeparturesTime = getCurrentTime();
		lastDeparturesTrain = train;
		LineWay& way;
		if (train.getTrainDirection() == FORWARD) way = forward;
		else way = backward;
		way.departures.pop();
		callDepartures(way);
	}
	
	
			
		
		
		
		
	
private:

	int lastDeparturesTime;
	int lastDeparturesTrain;

	int distance;
	std::string label;
	
	const int stationSpeedLimit;
	
	LineWay forward;
	LineWay backward;
	
	Track getTrack(TrackType type, LineWay& way) {
		for (Track track : way) if (track.getTrackType() == type && track.getTrackStatus() == FREE) return track;
		throw std::invalid_argument("Tnot found :( "); //EMPTY
	}
	
	bool isTrackFree(TrackType type, LineWay& way) {
		for (Track track : way) if (track.getTrackType() == type && track.getTrackStatus() == FREE) return true;
		return false;
	}
	
};




	//Track incomingTrain(Train t); //il treno in arrivo chiama la stazione
	//SE FERMATA
	//controllo se il treno è in anticipo/i binari sono occupati
	//se si, aggiungo a Arrivals() e rispondo con le informazioni (Il parcheggio è una track modificata?)
	//SE TRANSITO (Solo LocalStation derivata)
	//aggiungo a Transit() e rispondo con le informazioni (Il parcheggio è una track modificata?)
	
	
	//chiamata cadenziale per date a Transit e Arrival il via libera
	//(da capire come fare questa cosa!!) ---> Delega al treno?
	//chiamata quando un treno viene rimosso da Departures
	
	//quando un binario standard è libero, il primo di Arrivals parte
	//quando un binario transit è libero il primo di transit parte ---> si fermerà al binario.
	//in entrambi i casi, o al loro arrivo, o dopo 5 min dal loro arrivo verranno aggiunti a departures.
	//(da capire come fare questa cosa!!) ---> Delega al treno?
	
	//chiamata quando departures viene modificata!!!
	//quando aggiunti a departures se il primo in lista viene modificato, notifico il vecchio primo che è di nuovo in IDLE, ed il nuovo primo
	//gli dico dopo quando potrà partire


// Date due stazioni A -----> B distanti 50 Km

//Tempi di percorrenza complessiva Fermata -----> Fermata
//Serve che un treno arriva e quello dietro sia a 10 Km dalla stazione

//Treno Regionale ---> 5/80 + X-10/160 + 5/80 --> 7.5 + 15 = 22:30 minuti
//Treno AltaVeloc ---> 5/80 + X-10/240 + 5/80 --> 7.5 + 10 = 17:30 minuti
//Treno Altissima ---> 5/80 + X-10/300 + 5/80 --> 7.5 + 8  = 15:30 minuti

//Appena partito un Treno Regionale i tempi di attesa sono

//Treno Regionale STAZIONE ---> 5/80 + 5/160 ---> 3.75 + 1.875 = Parte dopo 5:27.5 minuti dal Regionale (Il regionale non guadagna spazio nel tempo)
//Treno AltaVeloc STAZIONE ---> (per 40km) 5/80 + 35/240 ---> 3.75 + 8.75 = 12:30 minuti ------> Parte dopo 10:00 dal Regionale
//Treno Altissima STAZIONE ---> (per 40km) 5/80 + 35/300 ---> 3.75 + 7    = 10:45 minuti ------> Parte dopo 11:45 dal Regionale

//Appena partito un Treno AltaVeloc i tempi di attesa sono

//Treno Regionale STAZIONE ---> 5/80 + 5/240 ---> 3.75 + 1.25 = Parte dopo 5:00 minuti dal AltaVeloc (Il regionale non guadagna spazio nel tempo)
//Treno AltaVeloc STAZIONE ---> 5/80 + 5/240 ---> 3.75 + 1.25 = Parte dopo 5:00 minuti dal AltaVeloc (Il altaveloc non guadagna spazio nel tempo)
//Treno Altissima STAZIONE ---> (per 40km) 5/80 + 35/300 ---> 3.75 + 7    = 10:45 minuti ------> Parte dopo 6:45 dal Altaveloc

//Appena partito un Treno Altissimo i tempi di attesa sono

//Treno Regionale STAZIONE ---> 5/80 + 5/300 ---> 3.75 + 1    = Parte dopo 4:45 minuti dal Altissimo (non guadagna spazio nel tempo)
//Treno AltaVeloc STAZIONE ---> 5/80 + 5/300 ---> 3.75 + 1    = Parte dopo 4:45 minuti dal Altissimo (non guadagna spazio nel tempo)
//Treno Altissima STAZIONE ---> 5/80 + 5/300 ---> 3.75 + 1    = Parte dopo 4:45 minuti dal Altissimo (non guadagna spazio nel tempo)

//Tempi di percorrenza complessiva Waiting/Fermata -----> Fermata
//Serve che un treno arriva e quello dietro sia a 10 Km dalla stazione

//Appena partito un Treno Regionale i tempi di attesa sono

//Treno AltaVeloc WAITING ---> (per 40+5km) 45/240 ---> = 11:15 minuti ------> Parte dopo 11:15 dal Regionale
//Treno Altissima WAITING ---> (per 40+5km) 45/300 ---> =  9:00 minuti ------> Parte dopo 13:30 dal Regionale

//Tempi di percorrenza complessiva Waiting/Waiting-Fermata -----> Fermata
//Serve che un treno arriva e quello dietro sia a 10 Km dalla stazione

//Appena partito un Treno AltaVeloc i tempi di attesa sono

//Treno AltaVeloc WAITING ---> 10/240 ---> = 2:30 minuti (non guadagna spazio nel tempo)
//Treno Altissima WAITING ---> (5/80 + 55/240) - (45/300) ---> 3.75+13.75-9 = 8:30 minuti ------> Parte dopo 8:30
//Treno Regionale STAZIONE ---> 15/240 ---> = 3:45 minuti (non guadagna spazio nel tempo)

//Appena partito un Treno Altissima i tempi di attesa sono

//Treno AltaVeloc WAITING ---> 10/300 ---> = 2:00 minuti (non guadagna spazio nel tempo)
//Treno Altissima WAITING ---> 10/300 ---> = 2:00 minuti (non guadagna spazio nel tempo)
//Treno Regionale STAZIONE ---> 15/300 ---> = 3:00 minuti (non guadagna spazio nel tempo)

//Ora pensando all'arrivo, le partenze fanno stimate come se il treno skippa!