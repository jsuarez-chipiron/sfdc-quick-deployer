cmake_minimum_required(VERSION 3.1)

project(quick-deployer-client)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

add_executable(sfdc-quick-deployer src/main.cpp src/sfdc_client/sfdc_client.cpp src/orquestrator/orquestrator.cpp
    src/resource_repo/resource_repo.cpp src/serdes/resource_serdes.cpp src/req_res_utils/req_res_utils.cpp)

target_link_libraries(sfdc-quick-deployer -lcurl -ltimer)
