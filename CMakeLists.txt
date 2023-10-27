cmake_minimum_required(VERSION 3.16)
project(fuzz-test)

enable_testing()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(cifuzz)
enable_fuzz_testing()

add_subdirectory(src)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE sut)

add_fuzz_test(my_fuzz_test my_fuzz_test_1.cpp)
target_link_libraries(my_fuzz_test PRIVATE sut)

add_fuzz_test(my_fuzz_test_old my_fuzz_test_2.cpp)
target_link_libraries(my_fuzz_test_old PRIVATE sut)
