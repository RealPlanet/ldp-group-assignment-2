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

    vector<string> tempFileLines = {};
    string newLine = "";

    while (getline(stations, newLine)) //Prendi una riga dal file
    {
        stringstream ss{ newLine }; //Converti la ringa in uno stream
        string tempString = "";
        string station_name = "";
        int distanza_da_origine = 0;

        // Station* newStation = new Station();
        //Ora nello stringstream rimangono solo gli orari quindi itero e salvo ogni orario come intero nel vettore
        while (ss >> tempString)
        {
            if (is_number(tempString)) //Se è un numero allora è per forza la distanza quindi convertila a numero
            {
                //Questo è la distanza della stazione dall'origine
                int distanza = stoi(tempString);
            }
            else
            {
                //Tutte le stringe fanno parte del nome della stazione
                
                station_name += tempString + " ";
            }
        }
        // TODO :: Rimuovere ultimo spazio quando si trova la distanza
        // TODO :: Aggiungere la stazione alla lista con le sue proprieta
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
