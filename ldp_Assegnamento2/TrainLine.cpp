
/*
    Fabio Ruscica - 1229076
*/

#include "TrainLine.h"
#include <string>
#include <sstream>
#include <fstream>
#include "generics.h"
#include "Station.h"

using std::isdigit;
using std::vector;
using std::string;
using std::ifstream;
using std::stringstream;
using std::stoi;

bool TrainLine::register_stations(std::string file_name)
{
    if (file_name == "")
    {
        return false;
    }

    ifstream stations(file_name);
    if (!stations.is_open())
    {
        return false;
    }

    string newLine = "";
    
    bool isOriginStation = true; //Con questo bool posso includere la prima stazione (caso degenere con un solo dato nel loop)
    while (getline(stations, newLine)) //Prendi una riga dal file
    {
        stringstream ss{ newLine }; //Converti la ringa in uno stream
        string tempString;
        string station_name = "";
        int station_type = 1;
        int originDistance = 0;
        // Station* newStation = new Station();
        //Ora nello stringstream rimangono solo gli orari quindi itero e salvo ogni orario come intero nel vettore
        while (ss >> tempString)
        {
            //Nella prima riga del fila c'� solo il nome della stazione principale, allora tutto � un nome
            if (is_number(tempString) && !isOriginStation) //Se � un numero allora � per forza la distanza quindi convertila a numero
            {
                // Primo numero � il tipo di stazione 0 o 1
                station_type = stoi(tempString);
                if (ss >> tempString && is_number(tempString))
                {
                    originDistance = stoi(tempString);
                }
                else
                {
                    throw InvalidStationDistance();
                }
            }
            else
            {
                //Tutte le stringe fanno parte del nome della stazione
                // TODO :: Rimuovere ultimo spazio quando si trova la distanza
                station_name += tempString + " ";
            }
        }

        if (isOriginStation)
        {
            Station* originalStation = new MainStation(originDistance, station_name);
            m_station_list = StationList();
            m_station_list.add(originalStation);
            isOriginStation = false; //Ora riprendiamo comportamento normale
        }
        
        Station* newStation;
        if (station_type)
        {
            newStation = new MainStation(originDistance, station_name);
        }
        else
        {
            newStation = new LocalStation(originDistance, station_name);
        }
        
        m_station_list.add(newStation);
    }

    stations.close(); //Chiudo lo stream e rilascio le risorse occupate
    return true;
}

StationInfo TrainLine::get_station_distances(int station_number, int starting_station, TrainType type) const
{
    StationInfo info = {};
    Station* s = nullptr;
    Station* s_p = nullptr;
    Station* s_n = nullptr;
    std::vector<Station*> allStationsVec = m_station_list.iterable();
    std::vector<int> mainStationsIndex = m_station_list.getIndexMain();
    int distance_prev = 0;
    int distance_next = 0;

    if (!starting_station) // Caso stazione iniziale 0
    {
        if (type == TrainType::REGIONALE)
        {
            s = allStationsVec[station_number];
            if (station_number > 0)
                s_p = allStationsVec[station_number - 1];
            if (station_number < mainStationsIndex.size() - 1)
                s_n = allStationsVec[station_number + 1];
        }
        else //Tutti gli altri tipi di treno sono considerati alta velocit�
        {
            s = allStationsVec[mainStationsIndex[station_number]];
            if( station_number > 0 ) 
                s_p = allStationsVec[mainStationsIndex[station_number - 1]];

            if (station_number < mainStationsIndex.size() - 1)
                s_n = allStationsVec[mainStationsIndex[station_number + 1]];
        }
        if (s_p) distance_prev = s->getDistance() - s_p->getDistance();
        if (s_n) distance_next = s_n->getDistance() - s->getDistance();
    }
    else // Caso stazione capolinea 1
    {
        station_number = allStationsVec.size() - 1 - station_number; //Inverto l'indice, quindi se richiedo la stazione zero e parto dal capoline ottengo la stazione uguale a vettiore[size - 1]
        if (type == TrainType::REGIONALE)
        {
            s = allStationsVec[station_number];
            if (station_number < allStationsVec.size() - 1 )
                s_p = allStationsVec[station_number + 1];
            if (station_number > 0)
                s_n = allStationsVec[station_number + 1];
        }
        else //Tutti gli altri tipi di treno sono considerati alta velocit�
        {
            s = allStationsVec[mainStationsIndex[station_number]];
            if (station_number < allStationsVec.size() - 1)
                s_p = allStationsVec[mainStationsIndex[station_number + 1]];
            if (station_number > 0)
                s_n = allStationsVec[mainStationsIndex[station_number + 1]];
        }
        // IN QUESTO CASO LA STAZIONE PRECEDENTE HA UNA MAGGIOR DISTANZA DALL'ORIGINE QUINDI LA FORMULA E' INVERTITA!!!
        if (s_p) distance_prev = s_p->getDistance() - s->getDistance();
        if (s_n) distance_next = s->getDistance()- s_n->getDistance();
    }

    info.m_next_station_distance = distance_next;
    info.m_prev_station_distance = distance_prev;
    return info;

    return info; // Caso pessimo non abbiamo trovato le informazioni richieste
}

/*
  Genera una lista di sole stazioni principali per i treni ad alta velocit�
*/
const StationList& TrainLine::get_main_station_list()
{
    std::vector<int> main_station_index = m_station_list.getIndexMain();
    std::vector<Station*> station_iterator = m_station_list.iterable();
    StationList main_stations = {};
    for (int i = 0; i < main_station_index.size(); i++)
    {
        main_stations.add(station_iterator[main_station_index[i]]);
    }

    return main_stations;
}

/*
    Verifica se la stringa � un numero oppure no, usato per registrare il nome delle varie stazioni
*/
bool TrainLine::is_number(const std::string& string) const
{
    // se il primo carattere � un numero stoi � in grado di convertirlo a numero
    return isdigit(string[0]);
}