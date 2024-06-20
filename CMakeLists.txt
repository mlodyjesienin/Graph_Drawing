cmake_minimum_required(VERSION 3.22)
project(Graph_Drawing)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(Graph_Drawing main.cpp
        graph.cpp
        GraphDisplay.cpp)

target_link_libraries(Graph_Drawing sfml-graphics sfml-window sfml-system)