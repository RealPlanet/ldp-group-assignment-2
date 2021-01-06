/*
	Fabio Ruscica - 1229076
*/

#ifndef transittrack_h
#define transittrack_h

#include "Track.h"

class TransitTrack : public Track
{
	TransitTrack() : Track(TrackType::TRANSIT) {}
};

#endif // !transittrack_h



