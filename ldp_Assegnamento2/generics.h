/*
	Fabio Ruscica
*/
#ifndef generics_h
#define generics_h
#include <vector>

enum class TrainType
{
	REGIONALE,
	ALTA_VELOCITA,
	ALTA_VELOCITA_SUPER
};

enum class BinaryType
{
	STANDARD,
	TRANSIT
};

enum class BinaryStatus
{
	OCCUPIED,
	FREE
};

struct TrainInfo
{
	int n_starting_station; // 0 == stazione origine, 1 == stazione capolinea
	TrainType m_train_type;
	std::vector<int> m_train_times;
};

#endif // !generics_h