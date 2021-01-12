// @author Matteo Salvalaio, 1216361

#include "include\TrainLine.h"
#include "include\TrainTime.h"
#include "include\Station.h"
#include "include\Train.h"

void run_simulation() {
	std::cout << "[WORKER] Generating the train line...\n\n";
	TrainLine* line = new TrainLine;                                          //Generation of the train line
	if (!line->register_stations("line_description.txt"))
	{
		std::cout << "[INFO] No line_description.txt file was found in root folder \n";
		return;
	}
	
	std::cout << "[WORKER] " << line->get_station_list().iterable().size() << " stations have been generated: ";
	for (Station* station : line->get_station_list().iterable()) std::cout << station->getLabel();
	std::cout << std::endl;
	
	std::cout << "\n[WORKER] Generating the time tables...\n\n";
	TrainTime* timetable = new TrainTime;                                     //Generation of the time tables
	if (!timetable->register_timetable("timetables.txt", line))
	{
		std::cout << "[INFO] No timetables.txt file was found in root folder \n";
		return;
	}
	
	std::cout << "\n[WORKER] Generating the trains...\n\n";
	
	std::vector<Train*> trains;
	for (int key : timetable->get_timetable_trains()) {                       //Generation of all the train from the timetable
		TrainInfo info = timetable->get_train_info(key);

		TrainDirection dir = TrainDirection::FORWARD;
		if(info.n_starting_station != 0) dir = TrainDirection::BACKWARD;
		if (info.m_train_type == TrainType::REGIONALE) trains.push_back(new RegionalTrain(key, dir, line, timetable));
		else if (info.m_train_type == TrainType::ALTA_VELOCITA) trains.push_back(new HighSpeedTrain(key, dir, line, timetable));
		else trains.push_back(new SuperHighSpeedTrain(key, dir, line, timetable));
		std::cout << "[INFO] Train " << key << " generated\n";
	}
	
	std::cout << "\n[WORKER] Starting the simulation...\n\n";
	
	int time = 0;
	while (trains.size() > 0) {                                              //The simulation keep running until there no more trains left
	
		/*
		for (int index = trains.size()-1; index >= 0; index--) {
			if (time%30==0)std::cout << "[" << (int)trains.at(index)->getDistance() << "]-" << trains.at(index)->getTrainID() << " ";
		}
		if (time%30==0)std::cout << "\n";
		*/
		
		for (int index = trains.size()-1; index >= 0; index--) {
			trains.at(index)->clock(time/60*100+time%60);                    //Call each train to update their position
			if (trains.at(index)->isEndline()) {                             //When a train is at it's final stop destroy it
				delete trains.at(index);
				trains.erase(trains.begin()+index);
			}
		}    
		for (Station* station : line->get_station_list().iterable()) {       //Call each station to update their time
			station->clock(time/60*100+time%60); 
		}
		time++;
	}
	delete line;
	delete timetable;
	std::cout << "\n[WORKER] Simulation ended...\n\n";
}

int main(int argc, char *argv[]) {
	run_simulation();                                                        //Start the simulation
	int t = 0;
	std::cout << "Premi un tasto per continuare...\n";
	std::cin >> t;
	return 0;
}