cmake_minimum_required (VERSION 3.5.2)

project(DuplicatesFinder LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_INSTALL_BINDIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_subdirectory(src)