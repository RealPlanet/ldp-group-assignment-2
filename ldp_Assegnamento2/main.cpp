// @author Matteo Salvalaio, 1216361

#include "TrainLine.h"
#include "TrainTime.h"
#include "Station.h"
#include "Train.h"

void run_simulation() {
	std::cout << "Generating the train line...\n";
	
	TrainLine* line = new TrainLine;
	line->register_stations("line_description.txt");
	
	std::cout << line->get_station_list().iterable().size() << " stations have been generated: ";
	for (Station* station : line->get_station_list().iterable()) std::cout << station->getLabel();
	std::cout << std::endl;
	
	TrainTime* timetable = new TrainTime;
	timetable->register_timetable("timetables.txt",line);                     //Generations of the train line and the timetables
	
	std::vector<Train*> trains;

	for (int key : timetable->get_timetable_trains()) {
		TrainInfo info = timetable->get_train_info(key);
		TrainDirection dir = TrainDirection::FORWARD;
		if(info.n_starting_station == 1) dir = TrainDirection::BACKWARD;
		                                                                     //Generate all the trains
		if (info.m_train_type == TrainType::REGIONALE) trains.push_back(new RegionalTrain(key, info.m_train_type, dir, line, timetable));
		else if (info.m_train_type == TrainType::ALTA_VELOCITA) trains.push_back(new HighSpeedTrain(key, info.m_train_type, dir, line, timetable));
		else trains.push_back(new SuperHighSpeedTrain(key, info.m_train_type, dir, line, timetable));
		
		
		std::cout << timetable->get_train_info(key).m_train_times.at(0) << "test ";
		
	}
	
	
	
	int time = 0;
	while (trains.size() > 0 && time < 1600) {  
								//The simulation keep running until there are still trains
		for (int index = trains.size()-1; index >= 0; index--) {
			if (time%30==0)std::cout << "[" << (int)trains.at(index)->getDistance() << "]-" << trains.at(index)->getTrainID() << " ";
		}
		if (time%30==0)std::cout << "\n";
		for (int index = trains.size()-1; index >= 0; index--) {
			trains.at(index)->clock(time/60*100+time%60);                    //Call each train to update their position
			if (trains.at(index)->isEndline()) {                             //When a train is at it's final stop destroy it
				delete trains.at(index);
				trains.erase(trains.begin()+index);
			}
		}    
          
			                                                 //Call each station to update their time
			for (Station* station : line->get_station_list().iterable()) {
				station->clock(time/60*100+time%60); 
			}
		time++;
		
	}
	std::cout << trains.size();
}

int main(int argc, char *argv[]) {
	run_simulation();                                                        //Start the simulation
	return 0;
}
