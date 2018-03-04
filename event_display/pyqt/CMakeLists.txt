cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(EVENT_DIS)

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

find_package(ROOT REQUIRED COMPONENTS)

include_directories($ENV{EVENT_DIS_BASEDIR}/core)
ROOT_GENERATE_DICTIONARY(G__Event Test.h LINKDEF LinkDef.h)

add_library(Event SHARED Event.cxx G__Event.cxx)
target_link_libraries(Event ${ROOT_LIBRARIES})
