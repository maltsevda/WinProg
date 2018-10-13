cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

# Dynamic Link Library

project(KeyboardHook)

set(SOURCES 
	KeyboardHook/KeyboardHook.cpp
	KeyboardHook/KeyboardHook.def
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_compile_definitions(${PROJECT_NAME} PUBLIC UNICODE _UNICODE)

# Executable

project(Launcher)

set(SOURCES 
	Launcher/main.cpp 
	Launcher/main.rc
)
set(HEADERS Launcher/resource.h)

add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${HEADERS})

target_compile_definitions(${PROJECT_NAME} PUBLIC UNICODE _UNICODE)
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
