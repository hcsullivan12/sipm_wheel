# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/hunter/Desktop/wheel/event_display/sipmwheel

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/hunter/Desktop/wheel/event_display/sipmwheel/build

# Include any dependencies generated for this target.
include CMakeFiles/SiPMWheel_CoreIO.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/SiPMWheel_CoreIO.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/SiPMWheel_CoreIO.dir/flags.make

SiPMWheel_CoreIO.cxx: ../core/io/Test.h
SiPMWheel_CoreIO.cxx: ../core/io/LinkDef.h
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/hunter/Desktop/wheel/event_display/sipmwheel/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating SiPMWheel_CoreIO.cxx"
	/home/hunter/Software/root-6.08.02/bin/rootcling -f SiPMWheel_CoreIO.cxx -c -I/home/hunter/Software/root-6.08.02/include -I/home/hunter/Desktop/wheel/event_display/sipmwheel/core -I/home/hunter/Desktop/wheel/event_display/sipmwheel/core/io /home/hunter/Desktop/wheel/event_display/sipmwheel/core/io/Test.h /home/hunter/Desktop/wheel/event_display/sipmwheel/core/io/LinkDef.h

CMakeFiles/SiPMWheel_CoreIO.dir/core/io/Test.cxx.o: CMakeFiles/SiPMWheel_CoreIO.dir/flags.make
CMakeFiles/SiPMWheel_CoreIO.dir/core/io/Test.cxx.o: ../core/io/Test.cxx
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hunter/Desktop/wheel/event_display/sipmwheel/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/SiPMWheel_CoreIO.dir/core/io/Test.cxx.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/SiPMWheel_CoreIO.dir/core/io/Test.cxx.o -c /home/hunter/Desktop/wheel/event_display/sipmwheel/core/io/Test.cxx

CMakeFiles/SiPMWheel_CoreIO.dir/core/io/Test.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SiPMWheel_CoreIO.dir/core/io/Test.cxx.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hunter/Desktop/wheel/event_display/sipmwheel/core/io/Test.cxx > CMakeFiles/SiPMWheel_CoreIO.dir/core/io/Test.cxx.i

CMakeFiles/SiPMWheel_CoreIO.dir/core/io/Test.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SiPMWheel_CoreIO.dir/core/io/Test.cxx.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hunter/Desktop/wheel/event_display/sipmwheel/core/io/Test.cxx -o CMakeFiles/SiPMWheel_CoreIO.dir/core/io/Test.cxx.s

CMakeFiles/SiPMWheel_CoreIO.dir/core/io/Test.cxx.o.requires:

.PHONY : CMakeFiles/SiPMWheel_CoreIO.dir/core/io/Test.cxx.o.requires

CMakeFiles/SiPMWheel_CoreIO.dir/core/io/Test.cxx.o.provides: CMakeFiles/SiPMWheel_CoreIO.dir/core/io/Test.cxx.o.requires
	$(MAKE) -f CMakeFiles/SiPMWheel_CoreIO.dir/build.make CMakeFiles/SiPMWheel_CoreIO.dir/core/io/Test.cxx.o.provides.build
.PHONY : CMakeFiles/SiPMWheel_CoreIO.dir/core/io/Test.cxx.o.provides

CMakeFiles/SiPMWheel_CoreIO.dir/core/io/Test.cxx.o.provides.build: CMakeFiles/SiPMWheel_CoreIO.dir/core/io/Test.cxx.o


CMakeFiles/SiPMWheel_CoreIO.dir/SiPMWheel_CoreIO.cxx.o: CMakeFiles/SiPMWheel_CoreIO.dir/flags.make
CMakeFiles/SiPMWheel_CoreIO.dir/SiPMWheel_CoreIO.cxx.o: SiPMWheel_CoreIO.cxx
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hunter/Desktop/wheel/event_display/sipmwheel/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/SiPMWheel_CoreIO.dir/SiPMWheel_CoreIO.cxx.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/SiPMWheel_CoreIO.dir/SiPMWheel_CoreIO.cxx.o -c /home/hunter/Desktop/wheel/event_display/sipmwheel/build/SiPMWheel_CoreIO.cxx

CMakeFiles/SiPMWheel_CoreIO.dir/SiPMWheel_CoreIO.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SiPMWheel_CoreIO.dir/SiPMWheel_CoreIO.cxx.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hunter/Desktop/wheel/event_display/sipmwheel/build/SiPMWheel_CoreIO.cxx > CMakeFiles/SiPMWheel_CoreIO.dir/SiPMWheel_CoreIO.cxx.i

CMakeFiles/SiPMWheel_CoreIO.dir/SiPMWheel_CoreIO.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SiPMWheel_CoreIO.dir/SiPMWheel_CoreIO.cxx.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hunter/Desktop/wheel/event_display/sipmwheel/build/SiPMWheel_CoreIO.cxx -o CMakeFiles/SiPMWheel_CoreIO.dir/SiPMWheel_CoreIO.cxx.s

CMakeFiles/SiPMWheel_CoreIO.dir/SiPMWheel_CoreIO.cxx.o.requires:

.PHONY : CMakeFiles/SiPMWheel_CoreIO.dir/SiPMWheel_CoreIO.cxx.o.requires

CMakeFiles/SiPMWheel_CoreIO.dir/SiPMWheel_CoreIO.cxx.o.provides: CMakeFiles/SiPMWheel_CoreIO.dir/SiPMWheel_CoreIO.cxx.o.requires
	$(MAKE) -f CMakeFiles/SiPMWheel_CoreIO.dir/build.make CMakeFiles/SiPMWheel_CoreIO.dir/SiPMWheel_CoreIO.cxx.o.provides.build
.PHONY : CMakeFiles/SiPMWheel_CoreIO.dir/SiPMWheel_CoreIO.cxx.o.provides

CMakeFiles/SiPMWheel_CoreIO.dir/SiPMWheel_CoreIO.cxx.o.provides.build: CMakeFiles/SiPMWheel_CoreIO.dir/SiPMWheel_CoreIO.cxx.o


# Object files for target SiPMWheel_CoreIO
SiPMWheel_CoreIO_OBJECTS = \
"CMakeFiles/SiPMWheel_CoreIO.dir/core/io/Test.cxx.o" \
"CMakeFiles/SiPMWheel_CoreIO.dir/SiPMWheel_CoreIO.cxx.o"

# External object files for target SiPMWheel_CoreIO
SiPMWheel_CoreIO_EXTERNAL_OBJECTS =

libSiPMWheel_CoreIO.so: CMakeFiles/SiPMWheel_CoreIO.dir/core/io/Test.cxx.o
libSiPMWheel_CoreIO.so: CMakeFiles/SiPMWheel_CoreIO.dir/SiPMWheel_CoreIO.cxx.o
libSiPMWheel_CoreIO.so: CMakeFiles/SiPMWheel_CoreIO.dir/build.make
libSiPMWheel_CoreIO.so: /home/hunter/Software/root-6.08.02/lib/libCore.so
libSiPMWheel_CoreIO.so: /home/hunter/Software/root-6.08.02/lib/libRIO.so
libSiPMWheel_CoreIO.so: /home/hunter/Software/root-6.08.02/lib/libNet.so
libSiPMWheel_CoreIO.so: /home/hunter/Software/root-6.08.02/lib/libHist.so
libSiPMWheel_CoreIO.so: /home/hunter/Software/root-6.08.02/lib/libGraf.so
libSiPMWheel_CoreIO.so: /home/hunter/Software/root-6.08.02/lib/libGraf3d.so
libSiPMWheel_CoreIO.so: /home/hunter/Software/root-6.08.02/lib/libGpad.so
libSiPMWheel_CoreIO.so: /home/hunter/Software/root-6.08.02/lib/libTree.so
libSiPMWheel_CoreIO.so: /home/hunter/Software/root-6.08.02/lib/libRint.so
libSiPMWheel_CoreIO.so: /home/hunter/Software/root-6.08.02/lib/libPostscript.so
libSiPMWheel_CoreIO.so: /home/hunter/Software/root-6.08.02/lib/libMatrix.so
libSiPMWheel_CoreIO.so: /home/hunter/Software/root-6.08.02/lib/libPhysics.so
libSiPMWheel_CoreIO.so: /home/hunter/Software/root-6.08.02/lib/libMathCore.so
libSiPMWheel_CoreIO.so: /home/hunter/Software/root-6.08.02/lib/libThread.so
libSiPMWheel_CoreIO.so: /home/hunter/Software/root-6.08.02/lib/libMultiProc.so
libSiPMWheel_CoreIO.so: CMakeFiles/SiPMWheel_CoreIO.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/hunter/Desktop/wheel/event_display/sipmwheel/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX shared library libSiPMWheel_CoreIO.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SiPMWheel_CoreIO.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/SiPMWheel_CoreIO.dir/build: libSiPMWheel_CoreIO.so

.PHONY : CMakeFiles/SiPMWheel_CoreIO.dir/build

CMakeFiles/SiPMWheel_CoreIO.dir/requires: CMakeFiles/SiPMWheel_CoreIO.dir/core/io/Test.cxx.o.requires
CMakeFiles/SiPMWheel_CoreIO.dir/requires: CMakeFiles/SiPMWheel_CoreIO.dir/SiPMWheel_CoreIO.cxx.o.requires

.PHONY : CMakeFiles/SiPMWheel_CoreIO.dir/requires

CMakeFiles/SiPMWheel_CoreIO.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/SiPMWheel_CoreIO.dir/cmake_clean.cmake
.PHONY : CMakeFiles/SiPMWheel_CoreIO.dir/clean

CMakeFiles/SiPMWheel_CoreIO.dir/depend: SiPMWheel_CoreIO.cxx
	cd /home/hunter/Desktop/wheel/event_display/sipmwheel/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hunter/Desktop/wheel/event_display/sipmwheel /home/hunter/Desktop/wheel/event_display/sipmwheel /home/hunter/Desktop/wheel/event_display/sipmwheel/build /home/hunter/Desktop/wheel/event_display/sipmwheel/build /home/hunter/Desktop/wheel/event_display/sipmwheel/build/CMakeFiles/SiPMWheel_CoreIO.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/SiPMWheel_CoreIO.dir/depend
