/*
    Fabio Ruscica
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include "TrainLine.h"
#include "TrainTime.h"

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
bool TrainTime::register_timetable(const std::string file_name, const TrainLine& line)
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

        int trainNumber;
        int tempTrainType;
        int trainStartingStation;
        int trainFirstDepartureTime;
        int tempTrainTime;
        
        // Salvo temporaneamente i dati in variabili
        ss >> trainNumber;
        ss >> trainStartingStation;
        ss >> tempTrainType;

        // Aggiungo i dati necessari allo struct newTrainInfo
        newTrainInfo.m_train_type = TrainType(tempTrainType);
        newTrainInfo.n_starting_station = trainStartingStation;
        newTrainInfo.m_train_times.push_back(trainFirstDepartureTime);
        //Ora nello stringstream rimangono solo gli orari quindi itero e salvo ogni orario come intero nel vettore
        for (int i = 0; ss >> tempTrainTime; i++)
        {
            int actualTime = is_valid_time(trainNumber, trainStartingStation, tempTrainTime, newTrainInfo.m_train_type, newTrainInfo.m_train_times, line);
            newTrainInfo.m_train_times.push_back(actualTime); 
        }

        while (newTrainInfo.m_train_times.size() < line.get_station_size())
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
    return m_timetable.size();
}

void TrainTime::update_train_time(const int train_number, const int station, const int newTime, bool isDelay)
{
    int time = abs(newTime); // Valore assoluto per conti successivi

    if (!isDelay) //Se è un ritardo allora si somma all'orario attuale, altrimenti si sottrae
    {
        time = 0 - time;
    }

    m_timetable.at(train_number).m_train_times[station] += newTime; //Aggiorna l'orario
}

int TrainTime::is_valid_time(const int& train_number, const int& trainStartingStation, const int& time, const TrainType& train_type, const std::vector<int>& timetable, const TrainLine& line) const
{
    int trainSpeed; // = Train.get_train_speed(train_type)
    if (timetable.size() > 0)
    {
        int prevDepartureTime = timetable[timetable.size() - 1];
    }
    else return time;
    StationInfo stationDistance = line.get_station_distances(timetable.size() - 1, trainStartingStation, train_type);
    float minTime = stationDistance.m_prev_station_distance / trainSpeed;
    if (time >= minTime)
    {
        return time;
    }
    else
    {
        float newTime = minTime + 60 * timetable.size() + 10;
        std::cout << "[INFO] Nuovo tempo calcolato per treno numero: " << train_number << ", tempo cambiato da: " << time << " a " <<< newTime;
        return newTime;
    }
}

Time convert_mil_to_time(const int time)
{
    std::string tempTime = std::to_string(time);
    char hour[2];
    char minutes[2];

    tempTime.copy(hour, 2, 0);
    tempTime.copy(hour, 2, 3);

    Time s;
    s.hour = atoi(hour);
    s.minutes = atoi(minutes);

    return s;
}

int convert_time_to_mil(Time time)
{
    stringstream s;
    string string_time = "";
    s << time.hour << time.minutes;
    string_time = s.str();

    return std::stoi(string_time);
}
