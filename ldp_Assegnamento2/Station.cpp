// @author Matteo Salvalaio, 1216361

#include "Station.h"

/**
 * @brief Destructor of the class station.
 */
Station::~Station() {
	for (Track* track : forward->tracks) delete track;
	for (Track* track : backward->tracks) delete track;
	delete forward;
	delete backward;
}

/**
 * @brief Set the next station.
 * @param station The next station pointer.
 * @throws InvalidStationDistance() If the two stations are too close (<20km).
 */
void Station::setNext(Station* station) {
	if (station->getDistance()-distance < 20) throw InvalidStationDistance();
	next = station;
}

/**
 * @brief Set the prevoius station.
 * @param station The previous station pointer.
 * @throws InvalidStationDistance() If the two stations are too close (<20km).
 */
void Station::setPrev(Station* station) {
	if (station->getDistance()-distance > -20) throw InvalidStationDistance();
	prev = station;
}

/**
 * @brief Get the next station of the list, if it exist.
 * @return The next station of the list.
 * @throws OutOfBoundsException() If you try to read an element out of the list.
 */
Station* Station::getNext() const {
	if(hasNext()) return next;
	throw OutOfBoundsException();
}

/**
 * @brief Get the previous station of the list, if it exist.
 * @return The previous station of the list.
 * @throws OutOfBoundsException() If you try to read an element out of the list.
 */
Station* Station::getPrev() const {
	if(hasPrev()) return prev;
	throw OutOfBoundsException();
}

/**
 * @brief When a train is ready to depart from the station it is put in a priority queue
 * and wait until he get the ok to depart.
 * @param train The train that called the function.
 * @throws ImpossibleDeparturesException() If a train try to depart from the last stop.
 */
void Station::eventOutgoingTrain(Train* train) {
	LineWay* way;
	if (train->getTrainDirection() == TrainDirection::FORWARD) way = forward;																			//FIXME getTrainDirection();
	else way = backward;
	
	if ((way == forward && !hasNext()) || (way == backward && !hasPrev())) throw ImpossibleDeparturesException();
	
	std::cout << "[ " << train->getName() << " ---call---> " << label << " ] The train is ready to depart";								//FIXME getName();
	way->departures.push(train);
	callDepartures(way);
}

/**
 * @brief Every minute check if any train from both direction can depart and periodically every
 * 15-4 (time limit for early train - time rounded to go to the station from the parking) 
 * minutes check if there are trains waiting to get a track on the station.
 * @param time Current time.
 */
void Station::clock(int time) {
	if (time < 0) throw InvalidTimeException();
	if (time%(15-4)) {
		callArrivals(forward);
		callArrivals(backward);
	}
	if (time > forward->nextTime && forward->departures.getSize()!=0) eventDepartedTrain(time, forward);
	if (time > backward->nextTime && backward->departures.getSize()!=0) eventDepartedTrain(time, backward);
}

/**
 * @brief If a train can depart it give him the signal to do so, and delete it from the
 * departing queue.
 * @param time Cuttent time of the simulation.
 * @param way Direction of the train.
 */
void Station::eventDepartedTrain(int time, LineWay* way) {
	if (time > way->nextTime && way->departures.getSize()!=0) {
		way->departures.top()->callTrain(StationSignal::DEPARTURE_ALLOW);																								//FIXME callTrain();
		std::cout << "[ " << way->departures.top()->getName() << " ---call---> " << label << " ] The train is departing from the station";				//FIXME getName();
		
		way->lastTime = time;
		way->lastTrain = way->departures.top();
		way->departures.pop();
		callDepartures(way);
	}
}


/**
 * @brief Check if a track is free and if the first train in queue is no more then 15 minutes early.
 * If so give the train the standard track to go to the station.
 * @param way The trains direction.
 */
void Station::callArrivals(LineWay* way) {
	if (way->arrivals.getSize() != 0) {
		if (isTrackFree(TrackType::STANDARD, way) && (way->arrivals.top()->getLate() < -aheadTolerace)) {												//FIXME
			Track* track = getTrack(TrackType::STANDARD, way);
			way->arrivals.top()->callTrain(StationSignal::ARRIVAL_ALLOW,track);																			//FIXME callTrain();
			
			std::cout << "[ " << label << " ---call---> " << way->arrivals.top()->getName() << " ] The train is allowed to go to the station";		//FIXME getName();
			track->update(TrackStatus::OCCUPIED);
			way->arrivals.pop();
		}
	}
}

/**
 * @brief Return the free track of the type requested.
 * @param type Type of track.
 * @param way The trains direction.
 * @return The track.
 * @throws FreeTrackNotFoundException() If there isn't any free track of the type requested.
 */
Track* Station::getTrack(TrackType type, LineWay* way) const {
	for (Track* track : way->tracks) if (track->type() == type && track->status() == TrackStatus::FREE) return track;
	throw FreeTrackNotFoundException();
}

/**
 * @brief Check if there is a free track of the type requested.
 * @param type Type of track.
 * @param way The trains direction.
 * @return True or false.
 */
bool Station::isTrackFree(TrackType type, LineWay* way) const {
	for (Track* track : way->tracks) if (track->type() == type && track->status() == TrackStatus::FREE) return true;
	return false;
}

/**
 * @brief Default constructor of the class MainStation.
 * @param d Distance of the station from the origin.
 * @param l Name of the station.
 */
MainStation::MainStation(int d, std::string l) : Station(d, l, StationType::MAIN) {
	forward->tracks.push_back(new Track(TrackType::STANDARD));
	forward->tracks.push_back(new Track(TrackType::STANDARD));
	backward->tracks.push_back(new Track(TrackType::STANDARD));
	backward->tracks.push_back(new Track(TrackType::STANDARD));
}

/**
 * @brief When a train is 20km incoming to the station it tell what he want to
 * do at the station. If the train want to stop at the station it check if there are
 * any free track and if the train is too early, if the train isn't early and there is a free track
 * he get the signal to proceed to the station at the track given; else he's added to the queue and need to wait
 * at the parking.
 * @param train The train that called the function.
 * @param request The request made to the station.
 * @throws InvalidRequestException() If an invalid request is given.
 */
void MainStation::eventIncomingTrain(Train* train, TrainRequest request) {
	LineWay* way;
	if (train->getTrainDirection() == TrainDirection::FORWARD) way = forward;																		//FIXME getTrainDirection();
	else way = backward;
		
	if (request == TrainRequest::STOP) {
		std::cout << "[ " << train->getName() << " ---call---> " << label << " ] for requesting a stop at the station";
		
		if (isTrackFree(TrackType::STANDARD, way) && (train->getLate() < -aheadTolerace)) {											//FIXME
			Track* track = getTrack(TrackType::STANDARD, way);
			train->callTrain(StationSignal::ARRIVAL_ALLOW,track);																						//FIXME callTrain();
			track->update(TrackStatus::OCCUPIED);
			
			std::cout << "[ " << label << " ---call---> " << train->getName() << " ] The train is allowed to go to the station";
			return;
		}
		way->arrivals.push(train);
		train->callTrain(StationSignal::ARRIVAL_DENY);																											//FIXME callTrain();
		std::cout << "[ " << label << " ---call---> " << train->getName() << " ] The train must go to the parking of the station";
		return;
	}
	throw InvalidRequestException();
}

/**
 * @brief If there are trains waiting to depart, calculate the time when the next
 * train can depart.
 * @param way The trains direction.
 */
void MainStation::callDepartures(LineWay* way) {
	if (way->lastTime == -1) way->nextTime = 0;
	if (way->departures.getSize() != 0) {
		int rawPrev = way->lastTrain->getMaxSpeed()*(abs(getNext()->getDistance()-distance)-10) + 5*stationSpeedLimit;							//FIXME getMaxSpeed()
		int rawNext = way->departures.top()->getMaxSpeed()*(abs(getNext()->getDistance()-distance)-20) + 5*stationSpeedLimit;				//FIXME getMaxSpeed()

		way->nextTime = way->lastTime+(rawPrev-rawNext);
	}
}

/**
 * @brief Default constructor of the class LocalStation.
 * @param d Distance of the station from the origin.
 * @param l Name of the station.
 */
LocalStation::LocalStation(int d, std::string l) : Station(d, l, StationType::LOCAL) {
	forward->tracks.push_back(new Track(TrackType::STANDARD));
	forward->tracks.push_back(new Track(TrackType::STANDARD));
	backward->tracks.push_back(new Track(TrackType::STANDARD));
	backward->tracks.push_back(new Track(TrackType::STANDARD));
	forward->tracks.push_back(new Track(TrackType::TRANSIT));
	backward->tracks.push_back(new Track(TrackType::TRANSIT));
}


/**
 * @brief When a train is 20km incoming to the station it tell what he want to
 * do at the station. If the train want to stop at the station it check if there are
 * any free track and if the train is too early, if the train isn't early and there is a free track
 * he get the signal to proceed to the station at the track given; else he's added to the queue and need to wait
 * at the parking.
 * If the train want to transit it is added to the departures queue and he wait at the parking
 * @param train The train that called the function.
 * @param request The request made to the station.
 */
void LocalStation::eventIncomingTrain(Train* train, TrainRequest request) {
	LineWay* way;
	if (train->getTrainDirection() == TrainDirection::FORWARD) way = forward;																		//FIXME getTrainDirection();
	else way = backward;
		
	if (request == TrainRequest::STOP) {
		std::cout << "[ " << train->getName() << " ---call---> " << label << " ] for requesting a stop at the station";
		
		if (isTrackFree(TrackType::STANDARD, way) && (train->getLate() < -aheadTolerace)) {											//FIXME
			Track* track = getTrack(TrackType::STANDARD, way);
			train->callTrain(StationSignal::ARRIVAL_ALLOW,track);																						//FIXME callTrain();
			track->update(TrackStatus::OCCUPIED);
			
			std::cout << "[ " << label << " ---call---> " << train->getName() << " ] The train is allowed to go to the station";
			return;
		}
		way->arrivals.push(train);
		train->callTrain(StationSignal::ARRIVAL_DENY);																											//FIXME callTrain();
		std::cout << "[ " << label << " ---call---> " << train->getName() << " ] The train must go to the parking of the station";
		return;
	}
	way->departures.push(train);
	callDepartures(way);
	std::cout << "[ " << label << " ---call---> " << train->getName() << " ] The train must go to the parking of the station";
}

/**
 * @brief If there are trains waiting to depart, calculate the time when the next
 * train can depart.
 * @param way The trains direction.
 */
void LocalStation::callDepartures(LineWay* way) {
	if (way->lastTime == -1) way->nextTime = 0;
	if (way->departures.getSize() != 0) {
		if (way->departures.top()->getTrainType() == TrainType::REGIONALE) {																					//FIXME getTrainType();
			int rawPrev = way->lastTrain->getMaxSpeed()*(abs(getNext()->getDistance()-distance)-10) + 5*stationSpeedLimit;							//FIXME getMaxSpeed()
			int rawNext = way->departures.top()->getMaxSpeed()*(abs(getNext()->getDistance()-distance)-20) + 5*stationSpeedLimit;				//FIXME getMaxSpeed()
			way->nextTime = way->lastTime+(rawPrev-rawNext);
			return;
		}
	}
	else {
		int rawPrev = way->lastTrain->getMaxSpeed()*(abs(getNext()->getDistance()-distance)-10) + 5*stationSpeedLimit;							//FIXME getMaxSpeed()
		int rawNext = way->departures.top()->getMaxSpeed()*(abs(getNext()->getDistance()-distance)-10);
		way->nextTime = way->lastTime+(rawPrev-rawNext);
	}
}