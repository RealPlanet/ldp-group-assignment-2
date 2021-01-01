#include "TrainLine.h"
#include <string>
#include <sstream>
#include <fstream>
#include "generics.h"

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
    
    //Station* originStation = new Station(station_name, 1, 0);

    bool isOriginStation = true; //Con questo bool posso includere la prima stazione (caso degenere con un solo dato nel loop)
    while (getline(stations, newLine)) //Prendi una riga dal file
    {
        stringstream ss{ newLine }; //Converti la ringa in uno stream
        string tempString;
        string station_name = "";
        int station_type = 1;
        double distanza_da_origine = 0.0f;
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
                    distanza_da_origine = stoi(tempString);
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
            //Station* originalStation = new Station(station_name, station_type, distanza_da_origine);
            //m_station_list = StationList(originalStation);
        }

        isOriginStation = false; //Ora riprendiamo comportamento normale
        //Station* newStation = new Station(station_name, station_type, distanza_da_origine);
        //m_station_list.addLast(newStation);
    }

    stations.close(); //Chiudo lo stream e rilascio le risorce occupate
    return true;
}

/*
    Verifica se la stringa è un numero oppure no, usato per registrare il nome delle varie stazioni
*/
bool TrainLine::is_number(const std::string& string) const
{
    // se il primo carattere è un numero stoi è in grado di convertirlo a numero
    return isdigit(string[0]);
}


