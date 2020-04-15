cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

# Lazy DLL

project(LazyDLL)

set(SOURCES 
	LazyDll/LazyDll.cpp
	LazyDll/LazyDll.h
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_compile_definitions(${PROJECT_NAME} PUBLIC UNICODE _UNICODE)

# Implicit DLL

project(ImplicitDLL)

set(SOURCES 
	ImplicitDll/ImplicitDll.cpp
	ImplicitDll/ImplicitDll.h
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_compile_definitions(${PROJECT_NAME} PUBLIC UNICODE _UNICODE)

# Executable

project(Executable)

set(SOURCES 
	Executable/main.cpp 
	Executable/main.rc
)
set(HEADERS Executable/resource.h)

add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE ./ImplicitDll)
target_link_libraries(${PROJECT_NAME} PRIVATE ImplicitDLL)

target_compile_definitions(${PROJECT_NAME} PUBLIC UNICODE _UNICODE)
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
