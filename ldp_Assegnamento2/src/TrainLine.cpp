
/*
    Fabio Ruscica - 1229076
*/
#include "..\include\generics.h"
#include "..\include\TrainLine.h"
#include "..\include\Station.h"
#include "..\include\Train.h"

#include <string>
#include <sstream>
#include <fstream>

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
        if (newLine == "") //Ferma la registrazione se si arriva ad una riga vuota (es. viene riga vuota alla fine del fine)
        {
            std::cout << "[INFO] Reached EOF for train description file \n";
            break;
        }
        stringstream ss{ newLine }; //Converti la ringa in uno stream
        string tempString;
        string station_name = "";
        int station_type = 1;
        int originDistance = 0;
        bool invalidStation = false; //Usato per determinare se una stazione è valida o meno

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
                    if (originDistance - m_station_list.getLast()->getDistance() <= 20)
                    {
                        std::cout << "[INFO] Stazione: " << station_name << " e\' stata rimossa perche\' non rispetta la distanza minima di 20Km! \n";
                        invalidStation = true;
                        break;
                    }
                }
                else
                {
                    //Se la distanza di una stazione contiene una lettara prima di ogni numero non è possibile convertire a int
                    throw InvalidStationDistance();
                }
            }
            else
            {
                //Tutte le stringe fanno parte del nome della stazione
                station_name += tempString + " ";
            }
        }

        //Se la stazione in considerazione non è valida, scartiamo tutto e non la inseriamo nella lista
        if (!invalidStation)
        {
            if (isOriginStation) //Questa è la prima stazione, caso special che inizializza anche lal ista
            {
                Station* originalStation = new MainStation((float)originDistance, station_name);
                m_station_list = StationList();
                m_station_list.add(originalStation);
                isOriginStation = false; //Ora riprendiamo comportamento normale
            }
            else
            {
                //Genera la stazione in base al tipo
                Station* newStation;
                if (station_type)
                {
                    newStation = new MainStation((float)originDistance, station_name);
                }
                else
                {
                    newStation = new LocalStation((float)originDistance, station_name);
                }
                m_station_list.add(newStation);
            }  
        }
    }

    stations.close(); //Chiudo lo stream e rilascio le risorse occupate
    return true;
}

/*
* Calcola le distanze delle due stazioni successiva e precedente rispetto ad una stazione
*/
StationInfo TrainLine::get_station_distances(int station_number, int starting_station, TrainType type) const
{
    StationInfo info = {};
    Station* s = nullptr;
    Station* s_p = nullptr;
    Station* s_n = nullptr;
    std::vector<Station*> allStationsVec = m_station_list.iterable();
    std::vector<int> mainStationsIndex = m_station_list.getIndexMain();
    float distance_prev = 0;
    float distance_next = 0;

    if (!starting_station) // Caso stazione iniziale 0
    {
        if (type == TrainType::REGIONALE)
        {
            s = allStationsVec[station_number];
            if (station_number > 0)
                s_p = allStationsVec[(size_t)(station_number) - 1];
            if (station_number < mainStationsIndex.size() - 1)
                s_n = allStationsVec[(size_t)station_number + 1];
        }
        else //Tutti gli altri tipi di treno sono considerati alta velocità
        {
            s = allStationsVec[mainStationsIndex[station_number]];
            if( station_number > 0 ) 
                s_p = allStationsVec[mainStationsIndex[(size_t)station_number - 1]];

            if (station_number < mainStationsIndex.size() - 1)
                s_n = allStationsVec[mainStationsIndex[(size_t)station_number + 1]];
        }
        if (s_p) distance_prev = s->getDistance() - s_p->getDistance();
        if (s_n) distance_next = s_n->getDistance() - s->getDistance();
    }
    else // Caso stazione capolinea 1
    {
        if (type == TrainType::REGIONALE)
        {
            station_number = (int)allStationsVec.size() - 1 - station_number; //Inverto l'indice, quindi se richiedo la stazione zero e parto dal capoline ottengo la stazione uguale a vettiore[size - 1]

            s = allStationsVec[station_number];
            if (station_number < allStationsVec.size() - 1 )
                s_p = allStationsVec[(size_t)station_number + 1];
            if (station_number > 0)
                s_n = allStationsVec[(size_t)station_number + 1];
        }
        else //Tutti gli altri tipi di treno sono considerati alta velocità
        {
            station_number = (int)mainStationsIndex.size() - 1 - station_number; //Inverto l'indice, quindi se richiedo la stazione zero e parto dal capoline ottengo la stazione uguale a vettiore[size - 1]

            s = allStationsVec[mainStationsIndex[station_number]];
            if (station_number < allStationsVec.size() - 1)
                s_p = allStationsVec[mainStationsIndex[(size_t)station_number + 1]];
            if (station_number > 0)
                s_n = allStationsVec[mainStationsIndex[(size_t)station_number + 1]];
        }
        // IN QUESTO CASO LA STAZIONE PRECEDENTE HA UNA MAGGIOR DISTANZA DALL'ORIGINE QUINDI LA FORMULA E' INVERTITA!!!
        if (s_p) distance_prev = s_p->getDistance() - s->getDistance();
        if (s_n) distance_next = s->getDistance()- s_n->getDistance();
    }

    info.m_next_station_distance = distance_next;
    info.m_prev_station_distance = distance_prev;

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