cmake_minimum_required(VERSION 3.23)
project(console)

set(CMAKE_CXX_STANDARD 14)

add_executable(console main.cpp Console.cpp Console.h)

FIND_PACKAGE(nlohmann_json 3.11.2 REQUIRED)
FIND_PACKAGE(Boost 1.81 COMPONENTS program_options REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

add_executable(Console main.cpp Console.cpp Console.h InvalidResponse.h)
TARGET_LINK_LIBRARIES(Console LINK_PUBLIC ${Boost_LIBRARIES} )
target_link_libraries(Console PRIVATE nlohmann_json::nlohmann_json)