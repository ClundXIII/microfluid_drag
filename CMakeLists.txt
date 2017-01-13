
project(microfluid_drag)
cmake_minimum_required(VERSION 2.8)

# add header files of this module
set(HEADERS
    ${PROJECT_SOURCE_DIR}/src/define.h
    ${PROJECT_SOURCE_DIR}/src/cell.h
    ${PROJECT_SOURCE_DIR}/src/collision.h
    ${PROJECT_SOURCE_DIR}/src/lattice_boltzmann.h
    ${PROJECT_SOURCE_DIR}/src/fluid_simulation.h
)

# add source files of this module
set(SOURCES
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/cell.cpp
    ${PROJECT_SOURCE_DIR}/src/collision.cpp
    ${PROJECT_SOURCE_DIR}/src/lattice_boltzmann.cpp
    ${PROJECT_SOURCE_DIR}/src/fluid_simulation.cpp
)

#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC -g3 -fsanitize=address -Wall -pedantic -Wextra -fopenmp")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC -Wall -pedantic -Wextra -fopenmp -O3")

SET(GCC_COVERAGE_LINK_FLAGS    "-pthread -fopenmp")

#SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS} -fsanitize=address -g")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

add_executable(microfluid_drag ${HEADERS} ${SOURCES})
