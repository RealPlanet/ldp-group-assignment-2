cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(ldp_Assegnamento2 C CXX)

add_executable(main main.cpp)

# Fabio
add_library(TrainLine TrainLine.h TrainLine.cpp)
add_library(TrainTime TrainTime.h TrainTime.cpp)
add_library(Track Track.h Track.cpp)
add_library(StandardTrack StandardTrack.h StandardTrack.cpp)
add_library(TransitTrack TransitTrack.h TransitTrack.cpp)
add_library(generics generics.h generics.cpp)
# Aj
add_library(Train Train.h Train.cpp)
add_library(SuperHighSpeedTrain Train.h SuperHighSpeedTrain.cpp)
add_library(HighSpeedTrain Train.h HighSpeedTrain.cpp)
# Matteo
add_library(TrainPQ TrainPQ.h TrainPQ.cpp)
add_library(StationList StationList.h StationList.cpp)
add_library(Station Station.h Station.cpp)

target_link_libraries(main TrainLine)
target_link_libraries(main TrainTime)
target_link_libraries(main Track)
target_link_libraries(main StandardTrack)
target_link_libraries(main TransitTrack)
target_link_libraries(main generics)
target_link_libraries(main Train)
target_link_libraries(main HighSpeedTrain)
target_link_libraries(main SuperHighSpeedTrain)
target_link_libraries(main TrainPQ)
target_link_libraries(main StationList)
target_link_libraries(main Station)