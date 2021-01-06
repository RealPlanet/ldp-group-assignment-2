/*
	Fabio Ruscica - 1229076
*/

#ifndef standardtrack_h
#define standardtrack_h

#include "Track.h"
class StandardTrack : public Track
{
	StandardTrack() : Track(TrackType::STANDARD) {}
};

#endif // !standardtrack_h