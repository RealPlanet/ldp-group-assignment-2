/*
	Fabio Ruscica - 1229076
*/

#ifndef standardtrack_h
#define standardtrack_h

#include "Track.h"
/*
* Implementa Track
*/
class StandardTrack : public Track
{
	StandardTrack() : Track(TrackType::STANDARD) {}//Le funzioni base sono già implementate in track
};

#endif // !standardtrack_h