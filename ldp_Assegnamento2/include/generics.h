/*
	Fabio Ruscica - 1229076
*/

#ifndef generics_h
#define generics_h

#include <vector>
#include <string>
/*
	Header generico che contiene svariate strutture e enum usate in tutto il progetto
*/

/*
	START OF ENUMS
*/
// Tipi possibili di un treno
enum class TrainType
{
	NOT_A_TRAIN, //Placeholder
	REGIONALE,
	ALTA_VELOCITA,
	ALTA_VELOCITA_SUPER
};

/*
	Tipi possibili di stazioni
*/
enum class StationType
{
	LOCAL,
	MAIN
};

// Tipi possibili di un binario
enum class TrackType
{
	STANDARD,
	TRANSIT
};

// Stati possibili di un binario
enum class TrackStatus
{
	OCCUPIED,
	FREE
};

/*
	Direzioni che i treni possono avere rispetto ad una stazione generica
*/
enum class TrainDirection
{
	BACKWARD,
	FORWARD
};

/*
	Tipi di richiesta che un treno pu� fare ad una stazione
	STOP = Treno si ferma per gestire i passeggeri
	TRANSIT = Treno deve passare senza fermarsi
*/
enum class TrainRequest
{
	STOP,
	TRANSIT
};

/*
	Tipi di segnale che la stazione pu� inviare ad un treno
*/
enum class StationSignal
{
	ARRIVAL_ALLOW,
	ARRIVAL_DENY,
	DEPARTURE_ALLOW
};
/*
	END OF ENUMS
*/

// Struttura usata per contenere tutti gli orari e le informazione necessarie per un treno
struct TrainInfo
{
	int n_starting_station; // 0 == stazione origine, 1 == stazione capolinea
	TrainType m_train_type;
	std::vector<int> m_train_times;
};

struct StationInfo
{
	std::string m_station_name;
	float m_prev_station_distance;
	float m_next_station_distance;
};
#endif // !generics_h