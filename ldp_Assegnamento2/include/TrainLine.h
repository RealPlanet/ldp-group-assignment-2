/*
	Fabio Ruscica - 1229076
*/

#ifndef trainline_h
#define trainline_h

#include "StationList.h"
#include "generics.h"
#include "Station.h"
#include "Train.h"
#include <string>

class Station;
class Train;

class TrainLine
{
public:
    //Registra da un qualunque file di testo le stazioni di questa linea
	bool register_stations(std::string file_name = "");
    //Restituisce il numero di stazioni nella linea
	int get_station_size() const { return m_station_list.getSize(); }
    //Restituisce una reference costante per ispezione della linea
	const StationList& get_station_list() const { return m_station_list; }
    // Restituisce il numero di stazioni principali
	int get_main_station_size() const { return (int)m_station_list.getIndexMain().size(); }
    //Restituisce la distanza alla stazione precedente e successiva rispetto ad una stazione
	StationInfo get_station_distances(int station_number, int starting_station, TrainType type) const;
	//Eccezione usata nella classe
	class InvalidStationDistance : public std::exception {};
private:
	StationList m_station_list;
	bool is_number(const std::string& string) const;
};

//Restituisce la stazione successiva in base al tipo e alla direzzione del treno
static Station* get_next_main_station(Station* c_station, Train& train)
{
    bool f = train.getTrainDirection() == TrainDirection::FORWARD;
    bool condition = true;
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
        // Soluzione rapida, alternativamente si può implementare qualche function pointer per semplificare il loop e le sue condizioni
        condition = (f && c_station->hasNext()) || (!f && c_station->hasPrev());
    } while (condition && c_station->getStationType() != StationType::MAIN);

    return c_station;
}


#endif // !trainline_h


