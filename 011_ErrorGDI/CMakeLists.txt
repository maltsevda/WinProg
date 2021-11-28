cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

project(HelloWorld)

add_executable(${PROJECT_NAME} WIN32 main.c)

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})


