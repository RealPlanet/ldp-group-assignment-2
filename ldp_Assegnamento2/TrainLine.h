/*
	Fabio Ruscica - 1229076
*/

#ifndef trainline_h
#define trainline_h

#include "StationList.h"

class Station;
class TrainLine
{
public:
	bool register_stations(std::string file_name = "");
	int get_station_size() const { return m_station_list.size(); }
	StationInfo get_station_distances(int station_number, int starting_station, TrainType type) const;

	class InvalidStationDistance : public std::exception {};
private:
	StationList m_station_list;
	bool is_number(const std::string& string) const;
};

#endif // !trainline_h


