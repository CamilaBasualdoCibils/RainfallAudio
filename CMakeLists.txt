cmake_minimum_required(VERSION 3.10.0)
project(RainfallAudio VERSION 0.1.0 LANGUAGES C CXX)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(cmake/GL.cmake)
include(cmake/PortAudio.cmake)
include(cmake/Boost.cmake)
include(cmake/ImGui.cmake)
include(cmake/BrassUtils.cmake)
include(cmake/TaskFlow.cmake)
add_subdirectory(API)
add_subdirectory(App)