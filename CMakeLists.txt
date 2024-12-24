cmake_minimum_required(VERSION 3.29)
project(physics)

set(CMAKE_CXX_STANDARD 17)

set(SFML_DIR "/opt/homebrew/Cellar/sfml/2.6.2/lib/cmake/SFML")

find_package(SFML 2.6.2 COMPONENTS system window graphics network audio REQUIRED)
if (SFML_FOUND)
    message("SFML FOUND")
else()
    message(FATAL_ERROR "SFML NOT FOUND")
endif ()

include_directories(${SFML_INCLUDE_DIRS})
add_executable(physics main.cpp
        particle.h
        constraint.h
        input_handler.h)
target_link_libraries(physics sfml-audio sfml-network sfml-graphics sfml-window sfml-system)
