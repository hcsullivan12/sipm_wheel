# Install script for directory: /home/hunter/Desktop/wheel/event_display/sipmwheel

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/hunter/Desktop/wheel/event_display/sipmwheel/lib/SiPMWheel_CoreIO_rdict.pcm")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/hunter/Desktop/wheel/event_display/sipmwheel/lib" TYPE FILE FILES "/home/hunter/Desktop/wheel/event_display/sipmwheel/build/SiPMWheel_CoreIO_rdict.pcm")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/hunter/Desktop/wheel/event_display/sipmwheel/core/io/SiPMWheel_CoreIO.cxx")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/hunter/Desktop/wheel/event_display/sipmwheel/core/io" TYPE FILE FILES "/home/hunter/Desktop/wheel/event_display/sipmwheel/build/SiPMWheel_CoreIO.cxx")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if(EXISTS "$ENV{DESTDIR}/home/hunter/Desktop/wheel/event_display/sipmwheel/lib/libSiPMWheel_CoreIO.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/hunter/Desktop/wheel/event_display/sipmwheel/lib/libSiPMWheel_CoreIO.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/home/hunter/Desktop/wheel/event_display/sipmwheel/lib/libSiPMWheel_CoreIO.so"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/hunter/Desktop/wheel/event_display/sipmwheel/lib/libSiPMWheel_CoreIO.so")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/hunter/Desktop/wheel/event_display/sipmwheel/lib" TYPE SHARED_LIBRARY FILES "/home/hunter/Desktop/wheel/event_display/sipmwheel/build/libSiPMWheel_CoreIO.so")
  if(EXISTS "$ENV{DESTDIR}/home/hunter/Desktop/wheel/event_display/sipmwheel/lib/libSiPMWheel_CoreIO.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/hunter/Desktop/wheel/event_display/sipmwheel/lib/libSiPMWheel_CoreIO.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}/home/hunter/Desktop/wheel/event_display/sipmwheel/lib/libSiPMWheel_CoreIO.so"
         OLD_RPATH "/home/hunter/Software/root-6.08.02/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/home/hunter/Desktop/wheel/event_display/sipmwheel/lib/libSiPMWheel_CoreIO.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/hunter/Desktop/wheel/event_display/sipmwheel/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
