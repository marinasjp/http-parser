cmake_minimum_required(VERSION 3.10)

# set the project name
project(HTTPServer)

add_executable(HTTPServer server.cpp tcpServer.cpp)
target_include_directories(HTTPServer PRIVATE .)

target_link_libraries(HTTPServer libwsock32.a)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)