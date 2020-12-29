// @author Matteo Salvalaio, 1216361

#ifndef STATIONLIST_H
#define STATIONLIST_H

#include <vector>
#include "Station.h"

//Invariants
// - size >= 0

class StationList {
private:

	class StationNode {
	
	public:
		explicit StationNode(Station* s = new Station())			//Default Constructor
		: next{nullptr}, prev{nullptr}, station{s} {}
		
		~StationNode() {delete station;}							//Destructor
		
		StationNode(const StationNode& node) = delete;				//Copy Constructor
		StationNode(StationNode&& node) = delete;					//Move Constructor
		StationNode& operator=(const StationNode& node) = delete;	//Copy Assignment
		StationNode& operator=(StationNode&& node) = delete;		//Move Assignment

		void setNext(StationNode* node) {next = node;}				//Set the next node
		void setPrev(StationNode* node) {prev = node;}				//Set the previous node

		StationNode* getNext();										//Return the next node, if it exist
		StationNode* getPrev();										//Return the previous node, if it exist
		
		bool hasNext() {return next!=nullptr;}						//Check if this node has a subsequent node
		bool hasPrev() {return prev!=nullptr;}						//Check if this node has a precedent node
		
		Station* getStation() {return station;}						//Return the pointer to the station object
		
		class OutOfBoundsException : public std::exception {}		//Exception of the class StationNode
		
	private:
		StationNode* next;											//The next node
		StationNode* prev;											//The previous node
		Station* station;											//The station object
	};
	
	StationNode* first;															//The first station of the route
	StationNode* last;															//The last station of the route
	int size;																	//The total number of station in the route

	std::vector<StationNode*> Iterable();										//Return a list of all the nodes for inspection

public:
	explicit StationList(int s = 0)												//Default Constructor
	: first{nullptr}, last{nullptr}, size{s} {}
		
	~StationList() {for (StationNode* index : Iterable()) delete index;}		//Destructor

	StationList(const StationList& list);										//Copy Constructor
	StationList(StationList&& list);											//Move Constructor
	StationList& operator=(const StationList& list);							//Copy Assignment
	StationList& operator=(StationList&& list);									//Move Assignment
	
	Station* getFirst();														//Return the first station
	Station* getLast();															//Return the last station
	bool* isFirst(Station* station) {return station == getFirst();}				//Check if the station is the first
	bool* isLast(Station* station) {return station == getLast();}				//Check if the station is the last
	Station* next(Station* station);											//Return the next station, if it exist
	Station* prev(Station* station);											//Return the previous station, if it exist
	
	void addFirst(Station* station);											//Add a new first station
	void addLast(Station* station);												//Add a new last station
	
	void addBefore(Station* station, Station* beforeStation);					//Add a station before another
	void addAfter(Station* station, Station* afterStation);						//Add a station after another
	void remove(Station* station);											//Remove a station
	
	bool isEmpty();																//Check if the list is empty
	
	class EmptyListException : public std::exception {}							//Exception of the class StationList
	class InputNotFoundException : public std::exception {}					//Exception of the class StationList
};

#endif