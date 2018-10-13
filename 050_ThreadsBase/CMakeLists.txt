cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

project(ThreadsBase)
set(SOURCES main.cpp main.rc)
set(HEADERS resource.h)

add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${HEADERS})

target_compile_definitions(${PROJECT_NAME} PUBLIC UNICODE _UNICODE)
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})