cmake_minimum_required(VERSION 3.1)

project(quick-deployer-client)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

add_executable(main main.cpp sfdc_client.cpp)
target_link_libraries(main /usr/lib/libcurl.so)
