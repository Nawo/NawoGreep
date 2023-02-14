cmake_minimum_required(VERSION 3.18)

file(GLOB CLASSES *.cpp)

set(SOURCES ${CLASSES})

add_executable(NawoGrep
    ${SOURCES}
)