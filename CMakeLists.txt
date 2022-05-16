cmake_minimum_required(VERSION 3.17)
project(C++thread)

add_definitions(-std=c++11)

set(CMAKE_CXX_STANDARD 11)

SET(CMAKE_BUILD_TYPE "Debug")  
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")  
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")  

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)


link_libraries(-lpthread )
#set(CMAKE_CXX_FLAGS  "-lpthread")

add_subdirectory(src/AtomicDemo)
add_subdirectory(src/Chapter03)
add_subdirectory(src/Chapter04)
add_subdirectory(src/Chapter05)
add_subdirectory(src/Chapter06)
add_subdirectory(src/Chapter07)
add_subdirectory(src/Chapter08)
add_subdirectory(src/Chapter09)
