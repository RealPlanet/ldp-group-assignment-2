/*
	Fabio Ruscica - 1229076
*/

#ifndef track_h
#define track_h
#include "generics.h"

class Train;
class Track
{
public:
	Track(TrackType _type) : m_track_type{ _type }, m_current_status{TrackStatus::FREE}{}
	inline TrackType type() const { return m_track_type; }
	inline TrackStatus status(){ return m_current_status; }
	//Aggiorna lo stato del binario (OCCUPATO / LIBERO)
	inline void update(TrackStatus newStatus) { m_current_status = newStatus; }
private:
	const TrackType m_track_type;
	TrackStatus m_current_status;
};

#endif