cmake_minimum_required(VERSION 3.0.0)
project(shared_memory VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(consumer consumer.cpp)
add_executable(producer producer.cpp)

target_link_libraries(consumer rt pthread)
target_link_libraries(producer rt pthread)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
