cmake_minimum_required(VERSION 3.2)
project(FSM)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(FSM ${SOURCE_FILES})

set(BOOST_ROOT "/Users/zesst/boost/boost_1_58_0")
set(Boost_NO_SYSTEM_PATHS ON)
find_package(Boost COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

target_link_libraries(FSM ${Boost_LIBRARY_DIR})