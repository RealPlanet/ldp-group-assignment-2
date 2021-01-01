#pragma once
#include "StationList.h"

class Station;
class TrainLine
{
public:
	bool register_stations(std::string file_name = "");

	class InvalidStationDistance : public std::exception {};
private:
	StationList m_station_list;
	bool is_number(const std::string& string) const;
};

