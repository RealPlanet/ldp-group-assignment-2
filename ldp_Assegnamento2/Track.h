#pragma once
#include "generics.h"

class Train;

class Track
{
	Track(TrackType _type) : m_track_type{ _type }, m_current_status{TrackStatus::FREE}{}
	inline TrackType type() const { return m_track_type; }
	inline TrackStatus status(){ return m_current_status; }
	inline void update(TrackStatus newStatus) { m_current_status = newStatus; }
private:
	const TrackType m_track_type;
	TrackStatus m_current_status;
};