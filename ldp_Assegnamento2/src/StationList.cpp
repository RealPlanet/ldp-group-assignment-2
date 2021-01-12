// @author Matteo Salvalaio, 1216361

#include "..\include\StationList.h"
#include "..\include\Station.h"

/**
 * @brief Destructor of the class StationList.
 */
StationList::~StationList() {
	if (size != 0) for (Station* index : iterable()) delete index;
}

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
		last->setNext(station);
		station->setPrev(last);
	}
	else first = station;
	last = station;
	if (station->getStationType() == StationType::MAIN) mainIndex.push_back(size);
	size++;
}

/**
 * @brief Return an ordered list of all the stations.
 * @return The ordered list of all the stations.
 */
std::vector<Station*> StationList::iterable() const {
	if (isEmpty()) throw EmptyListException();
	std::vector<Station*> list;
	Station* index = first;
	list.push_back(index);
	while (index->hasNext()) {
		index = index->getNext();
		list.push_back(index);
	}
	return list;
}