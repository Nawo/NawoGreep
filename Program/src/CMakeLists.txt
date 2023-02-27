cmake_minimum_required(VERSION 3.18)
project(NawoGrep)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB CLASSES *.cpp)

set(SOURCES ${CLASSES})

add_executable(${PROJECT_NAME}
    ${SOURCES}
)