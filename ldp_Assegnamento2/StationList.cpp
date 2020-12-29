// @author Matteo Salvalaio, 1216361

#include "StationList.h"
#include <iostream>

StationList::StationNode* StationList::StationNode::getNext() {
	if(hasNext()) return next;
	throw std::invalid_argument("Has no next!"); // no next/prev
}

StationList::StationNode* StationList::StationNode::getPrev() {
	if(hasPrev()) return prev;
	throw std::invalid_argument("Has no next!"); // no next/prev
}

std::vector<StationList::StationNode*> StationList::Iterable() {
	if (isEmpty()) throw std::invalid_argument("The list is Empty!"); // EMPTY
	std::vector<StationList::StationNode*> list;
	StationList::StationNode* index = first;
	while (index != first) {
		list.push_back(index);
		index = index->getNext();
	}
	return list;
}

Station* StationList::getFirst() {
	if (isEmpty()) throw std::invalid_argument("The list is Empty!"); // EMPTY
	return first->getStation();
}

Station* StationList::getLast() {
	if (isEmpty()) throw std::invalid_argument("The list is Empty!"); //EMPTY
	return last->getStation();
}

Station* StationList::next(Station* station) {
	for (StationNode* index : Iterable()) {
		if (index->getStation() == station) return index->getNext()->getStation();
		index = index->getNext();
	}
	throw std::invalid_argument("Not in list"); //NOT IN LIST ERROR
}

Station* StationList::prev(Station* station) {
	for (StationNode* index : Iterable()) {
		if (index->getStation() == station) return index->getPrev()->getStation();
		index = index->getNext();
	}
	throw std::invalid_argument("Not in list"); //NOT IN LIST ERROR
}

void StationList::addFirst(Station* station) {
	StationList::StationNode* node = new StationList::StationNode(station);
	if (size != 0) {
		node->setNext(first);
		first->setPrev(node);
	}
	first = node;
	size++;
	}

void StationList::addLast(Station* station) {
	StationList::StationNode* node = new StationList::StationNode(station);
	if (size != 0) {
		node->setPrev(last);
		last->setNext(node);
	}
	last = node;
	size++;
	}

void StationList::addBefore(Station* station, Station* beforeStation) {
	StationList::StationNode* node = new StationList::StationNode(beforeStation);
	for (StationNode* index : Iterable()) {
		if (index->getStation() == station) {
			node->setNext(index);
			node->setPrev(index->getPrev());
			index->getPrev()->setNext(node);
			index->setPrev(node);
			size++;
		}
	}
	throw std::invalid_argument("not in the list"); // no next/prev
}

void StationList::addAfter(Station* station, Station* afterStation) {
	StationList::StationNode* node = new StationList::StationNode(afterStation);
	for (StationNode* index : Iterable()) {
		if (index->getStation() == station) {
			node->setPrev(index);
			node->setNext(index->getNext());
			index->getNext()->setPrev(node);
			index->setNext(node);
			size++;
		}
	}
	throw std::invalid_argument("not in list!"); // no next/prev
}

Station* StationList::remove(Station* station) {
	for (StationNode* index : Iterable()) {
		if (index->getStation() == station) {
			index->getNext()->setPrev(index->getPrev());
			index->getPrev()->setNext(index->getNext());
			delete index;
			size--;
		}
	}
	throw std::invalid_argument("notinlist!"); // no next/prev
}

bool StationList::isEmpty() {
		if (size == 0) return true;
		return false;
	}