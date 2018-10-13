cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

project(HelloWorld)
set(SOURCES proc.cpp main.cpp main.rc)
set(HEADERS main.h resource.h)

add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${HEADERS})

target_compile_definitions(${PROJECT_NAME} PUBLIC UNICODE _UNICODE)
target_compile_options(${PROJECT_NAME} PUBLIC /utf-8)
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})