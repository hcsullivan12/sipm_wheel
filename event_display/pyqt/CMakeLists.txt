cmake_minimum_required(VERSION 3.5)
project(event)

set(CMAKE_CXX_STANDARD 11)

list(APPEND CMAKE_MODULE_PATH $ENV{ROOTSYS}/etc/cmake)
find_package(ROOT COMPONENTS REQUIRED)
include_directories(${ROOT_INCLUDE_DIRS})
add_definitions(${ROOT_CXX_FLAGS})

include_directories(${PROJECT_SOURCE_DIR}/core)
include_directories(${PROJECT_SOURCE_DIR}/core/io)

file(GLOB_RECURSE sources ${PROJECT_SOURCE_DIR}/core/*.cxx)
file(GLOB_RECURSE headers ${PROJECT_SOURCE_DIR}/core/*.h)

#add_executable(event main.cxx ${sources} ${headers})

#target_link_libraries(event ${ROOT_LIBRARIES})

ROOT_GENERATE_DICTIONARY(Event_Dis_TestDict Test.h LINKDEF LinkDef.h)

add_library(Test SHARED ${PROJECT_SOURCE_DIR}/core/io/Test.cxx Event_Dis_TestDict)
target_link_libraries(Test ${ROOT_LIBRARIES})
install(TARGETS Test DESTINATION lib/)
