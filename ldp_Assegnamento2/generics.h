/*
	Fabio Ruscica
*/
#ifndef generics_h
#define generics_h
#include <vector>
/*
	Header generico che contiene svariate strutture e enum usate in tutto il progetto
*/
/*
	START OF ENUMS
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
	END OF ENUMS
*/

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