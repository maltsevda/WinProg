cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

# Run Time Library Sample

project(RunTime)

set(SOURCES
	RunTime/RunTime.cpp
	RunTime/RunTime.def
)
set(HEADERS RunTime/RunTime.h)

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})

target_compile_definitions(${PROJECT_NAME} PUBLIC UNICODE _UNICODE RUNTIME_EXPORTS)

# Load Time Library Sample

project(LoadTime)

set(SOURCES LoadTime/LoadTime.cpp)
set(HEADERS LoadTime/LoadTime.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})

target_compile_definitions(${PROJECT_NAME} PUBLIC UNICODE _UNICODE LOADTIME_EXPORTS)

# Executable

project(Executable)

set(SOURCES Exec/main.cpp)

add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
add_dependencies(${PROJECT_NAME} RunTime LoadTime)

target_include_directories(${PROJECT_NAME} PRIVATE ./RunTime)
target_include_directories(${PROJECT_NAME} PRIVATE ./LoadTime)
target_link_libraries(${PROJECT_NAME} PRIVATE LoadTime)

target_compile_definitions(${PROJECT_NAME} PUBLIC UNICODE _UNICODE)
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
