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
            //Nella prima riga del fila c'è solo il nome della stazione principale, allora tutto è un nome
            if (is_number(tempString) && !isOriginStation) //Se è un numero allora è per forza la distanza quindi convertila a numero
            {
                // Primo numero è il tipo di stazione 0 o 1
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
    /*
    * Itera su tutte le stazioni,
        i == numero di stazioni iterate -- j == numero di stazioni principali iterate

    */
    StationInfo info = {};
    if (!starting_station)
    {
        Station* node = m_station_list.getFirst();
        for (int i = 0, j = 0; true; i++)
        {
            
            if (type == TrainType::REGIONALE && i == station_number)
            {
                info.m_next_station_distance = node->getDistance();
                if(!i) info.m_prev_station_distance = node->getPrev()->getDistance(); //Se si tratta dalla stazione 1 allora non esiste distanza da quella precedente

                return info;
            }
            else if ( (type == TrainType::ALTA_VELOCITA || type == TrainType::ALTA_VELOCITA_SUPER) && j == station_number)
            {
                info.m_next_station_distance = node->getDistance();
                if (!i) info.m_prev_station_distance = node->getPrev()->getDistance(); //Se si tratta dalla stazione 1 allora non esiste distanza da quella precedente

                return info;
            }
        
            // Abbiamo passato una stazione principale allora incrementa j
            if (node->getStationType() == StationType::MAIN) j++;

            if (node->hasNext())
            {
                node = node->getNext(); //Abbiamo fatto le verifiche, proseguiamo al nodo successivo
            }
            else break;
        }
    }
    else
    {
        Station* node = m_station_list.getLast();
        for (int i = 0, j = 0; true; i++)
        {
            if (type == TrainType::REGIONALE && i == station_number)
            {
                info.m_next_station_distance = node->getPrev()->getDistance(); //Siccome percorriamo la lista al contrario dobbiamo tornare indietro per la distanza
                if (!i) info.m_prev_station_distance = node->getNext()->getDistance(); //Se si tratta dalla stazione N allora non esiste distanza da quella successiva
                return info;
            }
            else if ((type == TrainType::ALTA_VELOCITA || type == TrainType::ALTA_VELOCITA_SUPER) && j == station_number)
            {
                info.m_next_station_distance = node->getPrev()->getDistance(); //Siccome percorriamo la lista al contrario dobbiamo tornare indietro per la distanza
                if (!i) info.m_prev_station_distance = node->getNext()->getDistance(); //Se si tratta dalla stazione N allora non esiste distanza da quella successiva
                
                return info;
            }

            // Abbiamo passato una stazione principale allora incrementa j
            if (node->getStationType() == StationType::MAIN) j++;

            //Controllo se uscire o meno qui perchè devo controllare anche l'ultimo nodo della lista
            if (node->hasPrev())
            {
                node = node->getPrev(); //Abbiamo fatto le verifiche, proseguiamo al nodo successivo
            }
            else break;
        }
    }

    return info; // Caso pessimo non abbiamo trovato le informazioni richieste
}

/*
    Verifica se la stringa è un numero oppure no, usato per registrare il nome delle varie stazioni
*/
bool TrainLine::is_number(const std::string& string) const
{
    // se il primo carattere è un numero stoi è in grado di convertirlo a numero
    return isdigit(string[0]);
}


