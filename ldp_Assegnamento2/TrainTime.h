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
	bool register_timetable(const std::string file_name, const TrainLine& line);
	TrainInfo get_train_info(int train_number) const;
	int get_train_number() const;
	void update_train_time(const int train_number, const int station, const int newTime, bool isDelay);
private:
	std::map<int, TrainInfo> m_timetable;
	int is_valid_time(const int& train_number, const int& time, const TrainType& train_type, const std::vector<int>& timetable, const TrainLine& line) const;
};

#endif

