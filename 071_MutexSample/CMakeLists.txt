cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

project(MutexSample)
set(SOURCES main.c main.rc)
set(HEADERS resource.h)

add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${HEADERS})

target_compile_definitions(${PROJECT_NAME} PUBLIC UNICODE _UNICODE)
target_compile_options(${PROJECT_NAME} PUBLIC /utf-8)
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})