cmake_minimum_required(VERSION 3.10.2 FATAL_ERROR)
project(Sparse-Graph-Library)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

include_directories(include)
set(SOURCE_DIR "src")
file(GLOB SOURCES "${SOURCE_DIR}/*.cpp")

# Pybind related
add_subdirectory(pybind11)
set(PYBIND11_CPP_STANDARD -std=c++17)
pybind11_add_module(_sparse SHARED ${SOURCES})
set_property(TARGET _sparse PROPERTY CXX_STANDARD 17)
