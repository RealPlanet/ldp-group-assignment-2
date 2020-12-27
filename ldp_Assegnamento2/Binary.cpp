/*
	Fabio Ruscica
*/
#include "Binary.h"

bool Binary::isEmpty()
{
	return m_status == BinaryStatus::FREE;
}

BinaryStatus Binary::getStatus()
{
	return m_status;
}

BinaryType Binary::getType()
{
	return m_type;
}

void Binary::setStatus(BinaryStatus newStat)
{
	m_status = newStat;
}
