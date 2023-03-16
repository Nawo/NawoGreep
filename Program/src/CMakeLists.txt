cmake_minimum_required(VERSION 3.18)
project(NawoGrep)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        main # release-1.10.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(${PROJECT_NAME} main.cpp Grep.cpp FindedFiles.cpp)
add_executable(${PROJECT_NAME}-ut test.cpp Grep.cpp FindedFiles.cpp)

target_link_libraries(${PROJECT_NAME}-ut gtest_main)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}-ut)
