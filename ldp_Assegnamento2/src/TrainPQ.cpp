// @author Matteo Salvalaio, 1216361

#include "..\include\TrainPQ.h"
#include "..\include\Train.h"
#include "..\include\Station.h"
#include "..\include\Track.h"
#include "..\include\generics.h"

/**
 * @brief Add a new element to the queue.
 * @param train The element to add.
 */
void TrainPQ::push(Train* train) {
	for (int index = 0; index < trainsQueue.size(); index++) {
		if (compare(train, trainsQueue.at(index))) {
			trainsQueue.insert(trainsQueue.begin() + index, train);
			size++;
			return;
		}
	}
	trainsQueue.push_back(train);
	size++;
}

/**
 * @brief Return the first element of the list.
 * @return The first element of the list.
 * @throws EmptyListException() If the list is empty.
 */
Train* TrainPQ::top() {
	if (isEmpty()) throw EmptyListException();
	return trainsQueue.front();
}

/**
 * @brief Delete the first element of the list.
 * @throws EmptyListException() If the list is empty.
 */
void TrainPQ::pop() {
	if (isEmpty()) throw EmptyListException();
	trainsQueue.erase(trainsQueue.begin());
	size--;
}

/**
 * @brief Compare two elements and check between them which one have the most priority.
 * @param lth The first train to compare.
 * @param rth The first train to compare.
 * @return True if lth has more priority over rth, false otherwise.
 */
bool ArrivalsTrainPQ::compare(Train* lth, Train* rth) {
	int lthTime = lth->getNextArrivalTime();
	int rthTime = rth->getNextArrivalTime();
	
	if (lthTime/100*60+lthTime%100-rthTime/100*60+rthTime%100 > 0) return false;
	return true;
	}

/**
 * @brief Compare two elements and check between them which one have the most priority.
 * @param lth The first train to compare.
 * @param rth The first train to compare.
 * @return True if lth has more priority over rth, false otherwise.
 */
bool DeparturesTrainPQ::compare(Train* lth, Train* rth) {
	int lthTime = analyzeTime(lth);
	int rthTime = analyzeTime(rth);
	
	if (lthTime-rthTime > 0) return false;
	return true;
	}

/**
 * @brief Calculate the ipotetical time of arrival to the next
 * station if the train does not stop to the next station. @see comments on TrainPQ.h
 * @param train The train pointer.
 * @return The ipotetical time of arrival to the next station.
 */
int DeparturesTrainPQ::analyzeTime(Train* train) {
	int extraTime = 0;
	
	if (train->getTrainDirection()==TrainDirection::FORWARD) {
		if (train->getTrainType() != TrainType::REGIONALE) {
			Station* station = currentStation->getNext();
			while (station->getStationType() != StationType::MAIN) station = station->getNext();
			extraTime = (int)(station->getDistance()-currentStation->getNext()->getDistance() -5)/train->getMaxSpeed()*62+5/80*60;
		}
		return train->getNextArrivalTime()/100*60+train->getNextArrivalTime()%100-extraTime;
	}
	if (train->getTrainType() != TrainType::REGIONALE) {
		Station* station = currentStation->getPrev();
		while (station->getStationType() != StationType::MAIN) station = station->getPrev();
		extraTime = (int)(currentStation->getPrev()->getDistance()-station->getDistance() -5)/train->getMaxSpeed()*62+5/80*60;
	}
	return train->getNextArrivalTime()/100*60+train->getNextArrivalTime()%100-extraTime;
} 