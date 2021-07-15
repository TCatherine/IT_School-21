cmake_minimum_required(VERSION 3.0.0)
project(Task1 VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(Task1 main.cpp downloader.cpp)

target_link_libraries(${PROJECT_NAME} curl)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)