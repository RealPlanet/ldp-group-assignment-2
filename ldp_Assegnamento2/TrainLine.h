/*
	Fabio Ruscica - 1229076
*/

#ifndef trainline_h
#define trainline_h

#include "StationList.h"
#include "generics.h"
#include <string>

class Station;
class Train;

class TrainLine
{
public:
	bool register_stations(std::string file_name = "");
	int get_station_size() const { return m_station_list.getSize(); }
	const StationList& get_station_list() const { return m_station_list; }

	int get_main_station_size() const { return (int)m_station_list.getIndexMain().size(); }
	StationInfo get_station_distances(int station_number, int starting_station, TrainType type) const;
	

	class InvalidStationDistance : public std::exception {};
private:
	StationList m_station_list;
	bool is_number(const std::string& string) const;
};

static Station* get_next_main_station(Station* c_station, Train& train)
{
    do
    {
        if (train.getTrainDirection() == TrainDirection::FORWARD)
        {
            c_station = c_station->getNext();
        }
        else
        {
            c_station = c_station->getPrev();
        }
    } while (c_station->getStationType() != StationType::MAIN);

    return c_station;
}


#endif // !trainline_h


