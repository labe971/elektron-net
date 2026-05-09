cmake_minimum_required(VERSION 3.16)
project(elektron_miner CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_executable(elektron_miner miner.cpp)
target_link_libraries(elektron_miner PRIVATE Threads::Threads)

if(WIN32)
    target_link_libraries(elektron_miner PRIVATE winhttp)
endif()
