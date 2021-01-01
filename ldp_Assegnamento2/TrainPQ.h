// @author Matteo Salvalaio, 1216361

#ifndef TRAINPQ_H
#define TRAINPQ_H

#include "generics.h"
#include <iostream>
#include "Track.h"
#include <vector>

//PATTUME DA TOGLIERE//PATTUME DA TOGLIERE//PATTUME DA TOGLIERE//PATTUME DA TOGLIERE
class Train {//PATTUME DA TOGLIERE//PATTUME DA TOGLIERE//PATTUME DA TOGLIERE//PATTUME DA TOGLIERE
public://PATTUME DA TOGLIERE//PATTUME DA TOGLIERE//PATTUME DA TOGLIERE//PATTUME DA TOGLIERE
	TrainDirection getTrainDirection() {//PATTUME DA TOGLIERE//PATTUME DA TOGLIERE//PATTUME DA TOGLIERE
		return TrainDirection::FORWARD;//PATTUME DA TOGLIERE//PATTUME DA TOGLIERE//PATTUME DA TOGLIERE
	}//PATTUME DA TOGLIERE//PATTUME DA TOGLIERE//PATTUME DA TOGLIERE//PATTUME DA TOGLIERE
	int getMaxSpeed() {return 1;}//PATTUME DA TOGLIERE//PATTUME DA TOGLIERE//PATTUME DA TOGLIERE
	std::string getName() {return "i";}//PATTUME DA TOGLIERE//PATTUME DA TOGLIERE
    void callTrain(StationSignal si) {}//PATTUME DA TOGLIERE//PATTUME DA TOGLIERE//PATTUME DA TOGLIERE
	void callTrain(StationSignal si, Track* track) {}//PATTUME DA TOGLIERE//PATTUME DA TOGLIERE//PATTUME DA TOGLIERE
	int getLate() {return 1;}//PATTUME DA TOGLIERE//PATTUME DA TOGLIERE//PATTUME DA TOGLIERE//PATTUME DA TOGLIERE
	TrainType getTrainType() {return TrainType::REGIONALE;}//PATTUME DA TOGLIERE//PATTUME DA TOGLIERE
};//PATTUME DA TOGLIERE//PATTUME DA TOGLIERE//PATTUME DA TOGLIERE//PATTUME DA TOGLIERE
//PATTUME DA TOGLIERE//PATTUME DA TOGLIERE//PATTUME DA TOGLIERE//PATTUME DA TOGLIERE

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
	int size;                                                    //Dimension of the list
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
	explicit DeparturesTrainPQ() = default;                      //Default Constructor
private:
	bool compare(Train* lth, Train* rth) override;               //Implementation of compare from TrainPQ
};

#endif