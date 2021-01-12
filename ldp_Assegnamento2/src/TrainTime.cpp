/*
    Fabio Ruscica - 1229076
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include "..\include\TrainLine.h"
#include "..\include\TrainTime.h"
#include "..\include\Train.h"

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
        if (newLine == "") //Ferma la registrazione se si arriva ad una riga vuota (es. viene riga vuota alla fine del fine)
        {
            std::cout << "[INFO] Reached EOF for train times file \n";
            break;
        }

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

        // Se mancano orari vengono creati automaticamente
        while (newTrainInfo.m_train_times.size() < station_size)
        {
            int actualTime = is_valid_time(trainNumber, trainStartingStation, -1, newTrainInfo.m_train_type, newTrainInfo.m_train_times, line);
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
    //Ottieni le informazioni di un treno, lancia un'eccezione se out of bounds
    TrainInfo requested_info = m_timetable.at(train_number);
    return requested_info;
}

std::vector<int> TrainTime::get_timetable_trains()
{
    //Crea un vettore contenente le chiavi della mappa timetable
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
    int trainSpeed = getTrainSpeed(train_type);
    int prevDepartureTime = 0;
    if (timetable.size() > 0)
    {
        prevDepartureTime = timetable[timetable.size() - 1] / 100 * 60 + timetable[timetable.size() - 1] % 100;
    } // Se non esiste stazione un orario precedente allora questo tempo è per forza valido non si fà altro
    else return time;
    StationInfo stationDistance = line->get_station_distances((int)timetable.size(), trainStartingStation, train_type);
    // Fai un confronto senza considerare eventi che possono rallentare il treno (es: ritardi, limiti di velocità)
    int temptime = (time / 100 * 60 + time % 100) - prevDepartureTime;
    int minTime = (int)(stationDistance.m_prev_station_distance / trainSpeed * 60);

    if (temptime >= minTime)
    {
        return time;
    }
    else
    {
        //Tempo minimo di un treno per arrivare alla stazione consideranto possibili rallentamenti e un margine di 10 minuti
        int newTime = (prevDepartureTime + (int)minTime + 10) / 60 * 100 + (prevDepartureTime + (int)minTime + 10) % 60;
        if(time > 0) std::cout << "[INFO] Nuovo tempo calcolato per treno numero: " << train_number << ", tempo cambiato da: " << time << " a " << newTime << std::endl;
        return newTime;
    }
}

