/*
    Fabio Ruscica - 1229076
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include "TrainLine.h"
#include "TrainTime.h"
#include "Train.h"

using std::ifstream;
using std::ios;
using std::vector;
using std::string;
using std::stringstream;
using std::pair;

/*
    file_name == nome del file da aprire per leggere gli orari del treno
    restituisce vero se l'operazione va a buon fine altrimenti falso
*/
bool TrainTime::register_timetable(const std::string file_name, const TrainLine* line)
{
    if (file_name == "")
    {
        return false;
    }

    ifstream timetable(file_name);
    if (!timetable.is_open())
    {
        return false;
    }
    
    string newLine = "";

    while (getline(timetable, newLine)) //Prendi una riga dal file
    {
        stringstream ss{ newLine }; //Converti la ringa in uno stream
        TrainInfo newTrainInfo = {};

        int trainNumber = 0;
        int tempTrainType = 0;
        int trainStartingStation = 0;
        int tempTrainTime = 0;
        
        // Salvo temporaneamente i dati in variabili
        ss >> trainNumber;
        ss >> trainStartingStation;
        ss >> tempTrainType;
        ss >> tempTrainTime;
        // Aggiungo i dati necessari allo struct newTrainInfo
        newTrainInfo.m_train_type = TrainType(tempTrainType);
        newTrainInfo.n_starting_station = trainStartingStation;
        newTrainInfo.m_train_times.push_back(tempTrainTime);
        //Ora nello stringstream rimangono solo gli orari quindi itero e salvo ogni orario come intero nel vettore
        int station_size = line->get_station_size();
        if (newTrainInfo.m_train_type != TrainType::REGIONALE)
        {
            station_size = line->get_main_station_size();
        }

        for (int i = 1; ss >> tempTrainTime && i < station_size; i++)
        {
            int actualTime = is_valid_time(trainNumber, trainStartingStation, tempTrainTime, newTrainInfo.m_train_type, newTrainInfo.m_train_times, line);
            newTrainInfo.m_train_times.push_back(actualTime); 
        }

        while (newTrainInfo.m_train_times.size() < station_size)
        {
            int actualTime = is_valid_time(trainNumber, trainStartingStation, 0, newTrainInfo.m_train_type, newTrainInfo.m_train_times, line);
            newTrainInfo.m_train_times.push_back(actualTime);
        }

        /*
            Inserisco la coppia che associa il numero del treno alle sue informazioni nella mappa,
            come bonus la mappa è ordinata rispetto alle chiavi
        */
        m_timetable.insert(pair<int, TrainInfo>(trainNumber, newTrainInfo));
    }

    timetable.close(); //Chiudo lo stream e rilascio le risorce occupate
    return true;
}

TrainInfo TrainTime::get_train_info(int train_number) const
{
    // TODO :: Boundary check ??
    TrainInfo requested_info = m_timetable.at(train_number);

    return requested_info;
}

int TrainTime::get_train_number() const
{
    return (int)m_timetable.size();
}

void TrainTime::update_train_time(const int train_number, const int station, const int newTime, bool isDelay)
{
    int time = abs(newTime); // Valore assoluto per conti successivi

    if (!isDelay) //Se è un ritardo allora si somma all'orario attuale, altrimenti si sottrae
    {
        time = 0 - time;
    }

    std::cout << "[INFO] Aggiornato tempo della stazione " << station << "per il treno numero " << train_number;
    Time sTime = convert_mil_to_time(newTime);
    if (isDelay)
    {
        
        std::cout << " aggiunto ritardo di " << sTime.hour << "ore e " << sTime.minutes << "minuti";
    }
    else
    {
        std::cout << " treno in anticipo di " << sTime.hour << "ore e " << sTime.minutes << "minuti";
    }
    m_timetable.at(train_number).m_train_times[station] += newTime; //Aggiorna l'orario
}

std::vector<int> TrainTime::get_timetable_trains()
{
    std::vector<int> tempKey;

    auto cursor = m_timetable.begin();

    while (cursor != m_timetable.end())
    {
        tempKey.push_back( cursor->first );
        cursor++;
    }

    return tempKey;
}

int TrainTime::is_valid_time(const int& train_number, const int& trainStartingStation, const int& time, const TrainType& train_type, const std::vector<int>& timetable, const TrainLine* line)
{
    int trainSpeed = 100; //Train::getTrainSpeed(train_type);
    int prevDepartureTime = 0;
    if (timetable.size() > 0)
    {
        prevDepartureTime = timetable[timetable.size() - 1] / 100 * 60 + timetable[timetable.size() - 1] % 100;
    }
    else return time;
    StationInfo stationDistance = line->get_station_distances((int)timetable.size(), trainStartingStation, train_type);
    int temptime = (time / 100 * 60 + time % 100) - prevDepartureTime;
    int minTime = (int)stationDistance.m_prev_station_distance / (trainSpeed*60);
    if (temptime >= minTime)
    {
        return time;
    }
    else
    {
        int newTime = (prevDepartureTime + (int)minTime + 10) / 60 * 100 + (prevDepartureTime + (int)minTime + 10) % 60;
        std::cout << "[INFO] Nuovo tempo calcolato per treno numero: " << train_number << ", tempo cambiato da: " << time << " a " << newTime;
        return newTime;
    }
}

/*
* Funzione che permette di convertire i tempi letti da timetables.txt in uno struct che divide ore e minuti
*/
Time convert_mil_to_time(const int time)
{
    std::string tempTime = std::to_string(time);
    char hour[2] = { '\0' };
    char minutes[2] = { '\0' };

    tempTime.copy(hour, 2, 0);
    tempTime.copy(minutes, 2, 2);

    Time s = {};
    s.hour = atoi(hour);
    s.minutes = atoi(minutes);

    return s;
}

/*
* Funzione che permette di convertire uno struct Time in tempo militare
*/
int convert_time_to_mil(Time time)
{
    stringstream s = {};
    string string_time = "";
    s << time.hour << time.minutes;
    string_time = s.str();
    return std::stoi(string_time);
}

