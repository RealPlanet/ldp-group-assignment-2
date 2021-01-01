// @author Matteo Salvalaio, 1216361

#ifndef STATIONLIST_H
#define STATIONLIST_H

/************************************************
 *  La classe si occupa di definire i metodi per la
 *  creazione di una lista con posizioni astratte
 *  (Abstract Positional List).
 *  Alcuni metodi come quelli per la rimozioni di
 *  oggetti dalla lista non sono stati implementati
 *  in quanto non di utilità per il programma complessivo.
 *  Il costruttore di copia e l'assegnamento di copia
 *  sono stati disabilitati in quanto se lasciati
 *  default avrebbero prodotto lo stesso oggetto e non
 *  una copia e l'effettiva formazione di una copia di
 *  questa classe non è necessaria nel progetto.
 ***********************************************/

#include "Station.h"

class StationList {
public:
	explicit StationList()                                          //Default Constructor
	: first{nullptr}, last{nullptr} {}
		
	~StationList() {for (Station* index : iterable()) delete index;}//Destructor

	StationList(const StationList& list) = delete;                  //Copy Constructor
	StationList& operator=(const StationList& list) = delete;       //Copy Assignment
	StationList(StationList&& list) = default;                      //Move Constructor
	StationList& operator=(StationList&& list) = default;           //Move Assignment
	
	Station* getFirst() const;                                      //Return the first station
	Station* getLast() const;                                       //Return the last station
	
	void add(Station* station);                                     //Add a new station
	std::vector<Station*> iterable() const;                         //Return a list of all the stations
	
	bool isEmpty() const {return first==nullptr;}                   //Check if the list is empty
	
	class EmptyListException : public std::exception {};            //Exception of the class StationList
	
private:
	Station* first;                                                 //The first station of the route
	Station* last;                                                  //The last station of the route
};

#endif