#ifndef STATIONLIST_H
#define STATIONLIST_H

#include <vector>
#include <iostream>
#include "StationNode.h"

class StationList {
private:

	class StationNode {
	
	public:
		explicit StationNode(Station* s = new Station())
		: next{nullptr}, prev{nullptr}, station{s} {}
		
		~StationNode() {delete station;}
		
		StationNode(const StationNode& node) = delete;
		StationNode(StationNode&& node) = delete;
		StationNode& operator=(const StationNode& node) = delete;
		StationNode& operator=(StationNode&& node) = delete;

		void setNext(StationNode* node) {next = node;}
		void setPrev(StationNode* node) {prev = node;}

		StationNode* getNext();
		StationNode* getPrev();
		
		bool hasNext() {return next!=nullptr;}
		bool hasPrev() {return prev!=nullptr;}
		
		Station* getStation() {return station;}
	private:

		StationNode* next;
		StationNode* prev;
		Station* station;
	};
	
	StationNode* first;
	StationNode* last;
	int size;

	std::vector<StationNode*> Iterable();

public:

	explicit StationList(int s = 0)
	: first{nullptr}, last{nullptr}, size{s} {}
		
	~StationList() {for (StationNode* index : Iterable()) delete index;}

	StationList(const StationList& list) = delete;
	StationList(StationList&& list) = delete;
	StationList& operator=(const StationList& list) = delete;
	StationList& operator=(StationList&& list) = delete;
	
	Station* getFirst();
	Station* getLast();
	Station* next(Station* station);
	Station* prev(Station* station);
	
	void addFirst(Station* station);
	void addLast(Station* station);
	
	//TODO
	void addBefore(Station* station, Station* beforeStation);
	void addAfter(Station* station, Station* afterStation);
	Station* remove(Station* station);
	
	bool isEmpty() {
		if (size == 0) return true;
		return false;
	}
	

	};

#endif