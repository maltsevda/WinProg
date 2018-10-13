cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

project(Semaphore)

add_executable(${PROJECT_NAME} WIN32 main.cpp)

#target_compile_definitions(${PROJECT_NAME} PUBLIC UNICODE _UNICODE)
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
