// @author Matteo Salvalaio, 1216361

#include "StationList.h"

/**
 * @brief Get the first element of the list.
 * @return The first element of the list.
 * @throws EmptyListException() If the list is empty.
 */
Station* StationList::getFirst() const {
	if (isEmpty()) throw EmptyListException();
	return first;
}

/**
 * @brief Get the last element of the list.
 * @return The last element of the list.
 * @throws EmptyListException() If the list is empty.
 */
Station* StationList::getLast() const {
	if (isEmpty()) throw EmptyListException();
	return last;
}

/**
 * @brief Add a new station to the list.
 * @param station The station to add.
 */
void StationList::add(Station* station) {
	if (!isEmpty()) {
		getLast()->setNext(station);
		station->setPrev(getLast());
	}
	else first = station;
	last = station;
}

/**
 * @brief Return an ordered list of all the stations.
 * @return The ordered list of all the stations.
 */
std::vector<Station*> StationList::iterable() const {
	if (isEmpty()) throw EmptyListException();
	std::vector<Station*> list;
	Station* index = first;
	while (index->hasNext()) {
		list.push_back(index);
		index = index->getNext();
	}
	return list;
}