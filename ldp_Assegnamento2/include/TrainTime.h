/*
	Fabio Ruscica - 1229076
*/

#ifndef traintime_h
#define traintime_h

#include <map>
#include <string>
#include <vector>
#include "generics.h"

class TrainLine;
class TrainTime
{
public:
	//Registra un qualsiasi file come timetable dei treni
	bool register_timetable(const std::string file_name, const TrainLine* line);
	//Restituisce informazioni che riguardano un specifico treno (direzione, tipo e orari)
	TrainInfo get_train_info(int train_number) const;
	//Restituisce una const reference alla timetable per ispezione
	const std::map<int, TrainInfo>& get_timetable() const { return m_timetable; }
	//Restituisce un vettore contenente tutte le chiavi della timetable
	std::vector<int> get_timetable_trains();
	//Numero di treni (corrisponde alla grandezza della timetable)
	int get_train_number() const { return (int)m_timetable.size(); }
private:
	std::map<int, TrainInfo> m_timetable;
	//Funzione interna che verifica se un tempo registrato è valido
	int is_valid_time(const int& train_number, const int& starting_station, const int& time, const TrainType& train_type, const std::vector<int>& timetable, const TrainLine* line);
};
#endif

