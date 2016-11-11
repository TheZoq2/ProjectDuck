cmake_minimum_required(VERSION 3.3)
project(ProjectDuck)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
file(GLOB ProjectDuck_SRC 
    "*.cpp"
    "*.h"
    )

add_executable(duck ${PythonGenerator_SRC})
