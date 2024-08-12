cmake_minimum_required(VERSION 3.1...3.29)
set(CMAKE_CXX_STANDARD 11)

project(
  ccwc
  VERSION 1.0
  LANGUAGES CXX)

# Add CPM Dependency
include(../cmake/CPM.cmake)

CPMAddPackage(
  NAME CLI11
  GITHUB_REPOSITORY CLIUtils/CLI11
  GIT_TAG v2.4.2
  # DOWNLOAD_ONLY YES
)

include_directories(/opt/CLI11)

add_executable(ccwc ccwc.cpp)
target_link_libraries(ccwc CLI11::CLI11)

set_target_properties(ccwc PROPERTIES
            CXX_STANDARD 17)