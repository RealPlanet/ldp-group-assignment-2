/*
	Fabio Ruscica - 1229076
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
	int is_valid_time(const int& train_number, const int& starting_station, const int& time, const TrainType& train_type, const std::vector<int>& timetable, const TrainLine& line) const;
};

/*
*  Converti tempo militare (XXXX) in uno struct che divide ora e minuti
*/
static Time convert_mil_to_time(const int time);
/*
*  Converti struct Time in orario militare
*/
static int convert_time_to_mil(Time time);

#endif

