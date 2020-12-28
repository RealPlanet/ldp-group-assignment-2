/*
	Fabio Ruscica
*/
#ifndef generics_h
#define generics_h
#include <vector>
/*
	Header generico che contiene svariate strutture e enum usate in tutto il progetto
*/

// Tipi possibili di un treno
enum class TrainType
{
	NOT_A_TRAIN,
	REGIONALE,
	ALTA_VELOCITA,
	ALTA_VELOCITA_SUPER
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

// Struttura usata per contenere tutti gli orari e le informazione necessarie per un treno
struct TrainInfo
{
	int n_starting_station; // 0 == stazione origine, 1 == stazione capolinea
	TrainType m_train_type;
	std::vector<int> m_train_times;
};

/*
struct StationInfo
{
	std::string m_station_name;
	int n_dist_from_origin;
};


// Piccola struttura per agevolare la tesione dei tempi per i treni
struct Time
{
	int hour;
	int minute;
};
*/
#endif // !generics_h