// @author Matteo Salvalaio, 1216361

#include "TrainPQ.h"

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


//TODO!!!
bool ArrivalsTrainPQ::compare(Train* lth, Train* rth) {
	lth->
	
	}



//se true il mio (lth )ha precedenza
bool DeparturesTrainPQ::compare(Train* lth, Train* rth) {} //se true il mio (lth )ha precedenza