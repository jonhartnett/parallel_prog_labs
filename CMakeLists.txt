cmake_minimum_required(VERSION 3.9)
project(parallel_prog_labs)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(C_SOURCE_FILES io-util.h io-util.c util.h list.h rand.h rand.c)
set(CXX_SOURCE_FILES io-util.h io-util.cpp util.h list.h rand.h rand.cpp vector.cpp vector.h particle.cpp particle.h rand.cpp)
add_executable(histogram histogram.c ${C_SOURCE_FILES})
add_executable(monte monte.c ${C_SOURCE_FILES})
add_executable(mandelbrot mandelbrot.c ${C_SOURCE_FILES})
add_executable(n-body n-body.cpp ${CXX_SOURCE_FILES})

find_library(M_LIB m)
target_link_libraries(n-body ${M_LIB})

find_package(OpenMP)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")