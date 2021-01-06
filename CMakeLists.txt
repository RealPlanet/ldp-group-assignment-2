cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(ldp_Assegnamento2)

add_executable(main main.cpp)

# Fabio
add_library(TrainLine TrainLine.h TrainLine.cpp)
add_library(TrainTime TrainTime.h TrainTime.cpp)
add_library(Track Track.h)
add_library(StandardTrack StandardTrack.h)
add_library(TransitTrack TransitTrack.h)
add_library(generics generics.h)
# Aj
add_library(Train Train.h Train.cpp)
# Matteo
add_library(TrainPQ TrainPQ.h TrainPQ.cpp)
add_library(StationList StationList.h StationList.cpp)
add_library(Station Station.h Station.cpp)

target_link_libraries(TrainLine main)