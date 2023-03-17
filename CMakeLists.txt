cmake_minimum_required(VERSION 3.13)
project(moodMeConsole)

set(CMAKE_CXX_STANDARD 14)
set(JSON_Install OFF CACHE INTERNAL "")

find_package(nlohmann_json CONFIG  REQUIRED)
FIND_PACKAGE(Boost CONFIG COMPONENTS program_options REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

add_executable(moodMeConsole src/main.cpp src/invalidResponse.cpp src/client.cpp include/client.h)
TARGET_LINK_LIBRARIES(moodMeConsole LINK_PUBLIC ${Boost_LIBRARIES} )
target_link_libraries(moodMeConsole PRIVATE nlohmann_json::nlohmann_json)