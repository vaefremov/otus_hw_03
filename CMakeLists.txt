cmake_minimum_required(VERSION 3.5)

if("$ENV{TRAVIS_BUILD_NUMBER}" STREQUAL "")
  message("Warning: Could not find TRAVIS_BUILD_NUMBER environment variable")
  set(VERSION_NUMBER "0.1.42")
else()
  set(VERSION_NUMBER "0.1.$ENV{TRAVIS_BUILD_NUMBER}")
endif()

project(allocator VERSION ${VERSION_NUMBER})

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(benchmark)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

add_library(utilities src/my_container.cpp)
target_include_directories(utilities
    PRIVATE "${CMAKE_BINARY_DIR}"
    "include"
)

add_executable(allocator src/main.cpp)
target_include_directories(allocator
    PRIVATE 
    "include"
)
target_link_libraries(allocator PRIVATE utilities)

add_executable(test_allocator tests/test_allocator.cpp)
target_include_directories(test_allocator
    PRIVATE 
    "include"
    "src"
    "${CMAKE_BINARY_DIR}" "${CMAKE_HOME_DIRECTORY}"
    "${GTEST_INCLUDE_DIR}"
)
target_link_libraries(test_allocator
    utilities
    ${GTEST_LIBRARIES}
    ${GTEST_MAIN_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)

add_executable(benchmark_allocator tests/benchmark_allocator.cpp)
target_include_directories(benchmark_allocator
    PRIVATE 
    "include"
    "src"
    "${CMAKE_BINARY_DIR}" "${CMAKE_HOME_DIRECTORY}"
    "${GTEST_INCLUDE_DIR}"
)
target_link_libraries(benchmark_allocator
    utilities
    ${GTEST_LIBRARIES}
    ${GTEST_MAIN_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    benchmark::benchmark
)



enable_testing()
add_test(test_allocator bin/test_allocator)