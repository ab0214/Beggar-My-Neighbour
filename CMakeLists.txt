cmake_minimum_required(VERSION 3.10)
project(Beggar-My-Neighbour)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)

include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB SOURCES "src/*.cpp")
add_executable(BeggarMyNeighbour ${SOURCES})