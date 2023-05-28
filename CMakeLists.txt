cmake_minimum_required(VERSION 3.25.1)
project(pong)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-fPIC")

set(HEADER_FILES headers/Client.h headers/SerializedMessage.h headers/GameClient.h headers/PlayerData.h headers/GameContext.h headers/GameServer.h headers/Server.h headers/Network.h headers/PlayerConnection.h)
add_library(libClient src/Network.cpp src/Client.cpp src/GameClient.cpp src/GameContext.cpp src/SerializedMessage.cpp ${HEADER_FILES})
add_library(libServer src/Server.cpp src/Network.cpp src/PlayerConnection.cpp src/GameServer.cpp src/GameContext.cpp src/SerializedMessage.cpp ${HEADER_FILES})
target_include_directories(libClient PRIVATE headers)
target_include_directories(libServer PRIVATE headers)

target_link_libraries(libClient -lSDL2 -lboost_serialization)
target_link_libraries(libServer -lSDL2 -lboost_serialization)

add_executable(server src/main.cpp ${HEADER_FILES})
target_link_libraries(server libServer)

add_executable(client src/mainClient.cpp ${HEADER_FILES})
target_link_libraries(client libClient)

target_include_directories(client PRIVATE headers)
target_include_directories(server PRIVATE headers)