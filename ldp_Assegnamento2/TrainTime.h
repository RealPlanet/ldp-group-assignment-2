/*
	Fabio Ruscica
*/

#ifndef traintime_h
#define traintime_h

#include <map>
#include <string>
#include <vector>
#include "generics.h"

class TrainTime
{
public:
	bool register_timetable(std::string file_name = "");
	TrainInfo get_train_info(int train_number) const;
private:
	std::map<int, TrainInfo> m_timetable;
	bool is_valid_time(const int time) const;
};

#endif

