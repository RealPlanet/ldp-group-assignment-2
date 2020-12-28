/*
    Fabio Ruscica
*/

#include "TrainTime.h"
#include <iostream>
#include <fstream>
#include <sstream>

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
bool TrainTime::register_timetable(std::string file_name)
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
    
    vector<string> tempFileLines = {};
    string newLine = "";

    while (getline(timetable, newLine)) //Prendi una riga dal file
    {
        stringstream ss{ newLine }; //Converti la ringa in uno stream
        TrainInfo newTrainInfo = {};

        int trainNumber;
        int tempTrainType;
        int trainStartingStation;
        int tempTrainTime;
        
        // Salvo temporaneamente i dati in variabili
        ss >> trainNumber;
        ss >> trainStartingStation;
        ss >> tempTrainType;

        // Aggiungo i dati necessari allo struct newTrainInfo
        newTrainInfo.m_train_type = TrainType(tempTrainType);
        newTrainInfo.n_starting_station = trainStartingStation;
        //Ora nello stringstream rimangono solo gli orari quindi itero e salvo ogni orario come intero nel vettore
        while (ss >> tempTrainTime)
        {
            newTrainInfo.m_train_times.push_back(tempTrainTime);
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
    // TODO :: Boundary check
    TrainInfo requested_info = m_timetable.at(train_number);
    return requested_info;
}

bool TrainTime::is_valid_time(const int time) const
{
    // TODO implement function and use it in register_timetable per controllare la corretteza degli orari
    return true;
}
