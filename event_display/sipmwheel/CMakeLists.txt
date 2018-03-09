cmake_minimum_required(VERSION 3.5)
project(SIPM_WHEEL)

set(CMAKE_CXX_STANDARD 11)

list(APPEND CMAKE_MODULE_PATH $ENV{ROOTSYS}/etc/cmake)
find_package(ROOT COMPONENTS REQUIRED)
include_directories(${ROOT_INCLUDE_DIRS})
add_definitions(${ROOT_CXX_FLAGS})

include_directories(${PROJECT_SOURCE_DIR}/core)
include_directories(${PROJECT_SOURCE_DIR}/core/io)

file(GLOB_RECURSE sources ${PROJECT_SOURCE_DIR}/core/*.cxx)
file(GLOB_RECURSE headers ${PROJECT_SOURCE_DIR}/core/*.h)


ROOT_GENERATE_DICTIONARY(SiPMWheel_CoreIO Test.h LINKDEF LinkDef.h)

install(FILES
	${CMAKE_CURRENT_BINARY_DIR}/SiPMWheel_CoreIO_rdict.pcm
	#${CMAKE_CURRENT_BINARY_DIR}/libCustomClassesDict.rootmap
	DESTINATION ${PROJECT_SOURCE_DIR}/lib/)

install(FILES
	${CMAKE_CURRENT_BINARY_DIR}/SiPMWheel_CoreIO.cxx
	DESTINATION ${PROJECT_SOURCE_DIR}/core/io)

add_library(SiPMWheel_CoreIO SHARED ${PROJECT_SOURCE_DIR}/core/io/Test.cxx SiPMWheel_CoreIO)
target_link_libraries(SiPMWheel_CoreIO ${ROOT_LIBRARIES})
install(TARGETS SiPMWheel_CoreIO DESTINATION ${PROJECT_SOURCE_DIR}/lib/)
