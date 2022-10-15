cmake_minimum_required(VERSION 3.23)
project(LicensePlate)

set(CMAKE_CXX_STANDARD 14)

set(BOOST_ROOT /usr/include/boost)
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS system thread)

include_directories(${Boost_INCLUDE_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

add_executable(client   client/main.cpp
                        client/LicensePlateListener.h
                        client/SocketLicensePlateListener.cpp
                        client/SocketLicensePlateListener.h
                        client/PlateInfo.h client/TruckPassDeserializer.cpp client/TruckPassDeserializer.h)

target_link_libraries(client ${Boost_LIBRARIES})

add_executable(server server/main.cpp server/Server.h)
target_link_libraries(server ${Boost_LIBRARIES})