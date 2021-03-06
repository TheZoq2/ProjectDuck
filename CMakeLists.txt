cmake_minimum_required(VERSION 3.3)
project(ProjectDuck)

set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs -Wall -Wextra -Wno-unused -pedantic")
# set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
file(GLOB ProjectDuck_SRC
    "*.cpp"
    "*.hpp"
	"entities/*.cpp"
	"entities/*.hpp"
    )


set(EXECUTABLE_NAME "duck")

add_executable(${EXECUTABLE_NAME} ${ProjectDuck_SRC})

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2 REQUIRED system window graphics network audio)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} -lchipmunk)
endif()


