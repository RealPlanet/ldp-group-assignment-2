/*
	Fabio Ruscica
*/
#ifndef binary_h
#define binary_h
#include "generics.h"

class Binary
{
public:
	bool isEmpty();
	BinaryStatus getStatus();
	BinaryType getType();

	virtual void addTrain(/*Train newTrain*/) = 0;
	virtual void /*Train*/ removeTrain() = 0;
	
	class BinaryIsOccupied;
	class BinaryCannotBeOccupied;

private:
	BinaryType m_type;
	BinaryStatus m_status;
	//Train member class
	void setStatus(BinaryStatus newStat);
};

#endif
