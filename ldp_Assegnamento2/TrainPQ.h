// @author Matteo Salvalaio, 1216361

#ifndef TRAINPQ_H
#define TRAINPQ_H

#include "generics.h"
#include <iostream>
#include <vector>

class Train;
class Station;
class Track;

/************************************************
 *  La classe base delle priority queue che gestiscono
 *  l'ordine di priorità dei treni.
 ***********************************************/

//Invariants
// - size >= 0

class TrainPQ {
public:
	explicit TrainPQ() = default;                                //Default Constructor

	~TrainPQ() = default;                                        //Destructor
	TrainPQ(const TrainPQ& list) = delete;                       //Copy Constructor
	TrainPQ& operator=(const TrainPQ& list) = delete;            //Copy Assignment
	TrainPQ(TrainPQ&& list) = default;                           //Move Constructor
	TrainPQ& operator=(TrainPQ&& list) = default;                //Move Assignment
	
	Train* top();                                                //Return the first element of the list
	void pop();                                                  //Delete the first element of the list
	void push(Train* train);                                     //Add a new element to the list
	
	bool isEmpty() {return (size==0);}                           //Check if the list is empty
	int getSize() {return size;}                                 //Return the size of the queue
	
	class EmptyListException : public std::exception {};         //Exception of the class TrainPQ
	
protected:
	virtual bool compare(Train* lth, Train* rth) = 0;            //Compare two elements
	int size = 0;                                                //Dimension of the list
	std::vector<Train*> trainsQueue;                             //List of trains pointers
};

/************************************************
 *  Classi derivate di TrainPQ
 ***********************************************/

class ArrivalsTrainPQ : public TrainPQ {
public:
	explicit ArrivalsTrainPQ() = default;                        //Default Constructor
private:
	bool compare(Train* lth, Train* rth) override;               //Implementation of compare from TrainPQ
};

class DeparturesTrainPQ : public TrainPQ {
public:
	explicit DeparturesTrainPQ(Station* s = nullptr)
	: currentStation{s} {};                                      //Default Constructor
private:
	bool compare(Train* lth, Train* rth) override;               //Implementation of compare from TrainPQ
	int analyzeTime(Train* train);                               //Calculate the time the train must arrive at the next station for stop or transit
	const Station* currentStation;                               //Pointer to the current station
};

#endif

/************************************************
 * Informazioni e gestione delle priorità dei treni alle stazioni
 * 
 * Quando un treno è in attesa per entrare in una stazione per effettuare una
 * fermata, perchè la stazione è piena o il treno è eccessivamente in anticipo,
 * è inserito in ArrivalsTrainPQ.
 * 
 * ArrivalsTrainPQ confronta e dà priorità ai treni unicamente in base al loro
 * ritardo. Un treno che deve arrivare alle 15:00 avrà sicuramente più priorità
 * di uno il cui arrivo previsto è alle 15:30.
 * 
 * Quando un treno è in parcheggio pronto a transitare o in stazione in attesa
 * di partire, perchè la tratta successiva è attualmente occupata, è inserito
 * in DeparturesTrainPQ.
 * 
 * Nel caso i treni fermino alla stazione successiva il criterio di priorità è
 * lo stesso di ArrivalsTrainPQ, tuttavia se il treno non ferma alla prossima
 * stazione il programma calcola il tempo teorico massimo per il quale il treno
 * può arrivare alla prossima stazione senza arrivare alla stazione al quale si
 * ferma in ritardo, e gli assegna tale valore come valore teorico di arrivo
 * alla stazione successiva, in modo che possa essere comparato con gli altri
 * treni per determinare un ordine di priorità alla partenza.
 * 
 ************************************************/