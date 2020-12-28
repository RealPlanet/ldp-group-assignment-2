#pragma once
#include "StationList.h"

class TrainLine
{
public:
	bool register_stations(std::string file_name = "");
private:
	StationList m_station_list;
	bool is_number(const std::string& string) const;
};

