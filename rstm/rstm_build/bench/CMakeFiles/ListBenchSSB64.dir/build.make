# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /home/frantzcito/Desktop/rstm

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/frantzcito/Desktop/rstm/rstm_build

# Include any dependencies generated for this target.
include bench/CMakeFiles/ListBenchSSB64.dir/depend.make

# Include the progress variables for this target.
include bench/CMakeFiles/ListBenchSSB64.dir/progress.make

# Include the compile flags for this target's objects.
include bench/CMakeFiles/ListBenchSSB64.dir/flags.make

bench/CMakeFiles/ListBenchSSB64.dir/ListBench.cpp.o: bench/CMakeFiles/ListBenchSSB64.dir/flags.make
bench/CMakeFiles/ListBenchSSB64.dir/ListBench.cpp.o: ../bench/ListBench.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/frantzcito/Desktop/rstm/rstm_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object bench/CMakeFiles/ListBenchSSB64.dir/ListBench.cpp.o"
	cd /home/frantzcito/Desktop/rstm/rstm_build/bench && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ListBenchSSB64.dir/ListBench.cpp.o -c /home/frantzcito/Desktop/rstm/bench/ListBench.cpp

bench/CMakeFiles/ListBenchSSB64.dir/ListBench.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ListBenchSSB64.dir/ListBench.cpp.i"
	cd /home/frantzcito/Desktop/rstm/rstm_build/bench && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/frantzcito/Desktop/rstm/bench/ListBench.cpp > CMakeFiles/ListBenchSSB64.dir/ListBench.cpp.i

bench/CMakeFiles/ListBenchSSB64.dir/ListBench.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ListBenchSSB64.dir/ListBench.cpp.s"
	cd /home/frantzcito/Desktop/rstm/rstm_build/bench && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/frantzcito/Desktop/rstm/bench/ListBench.cpp -o CMakeFiles/ListBenchSSB64.dir/ListBench.cpp.s

bench/CMakeFiles/ListBenchSSB64.dir/ListBench.cpp.o.requires:

.PHONY : bench/CMakeFiles/ListBenchSSB64.dir/ListBench.cpp.o.requires

bench/CMakeFiles/ListBenchSSB64.dir/ListBench.cpp.o.provides: bench/CMakeFiles/ListBenchSSB64.dir/ListBench.cpp.o.requires
	$(MAKE) -f bench/CMakeFiles/ListBenchSSB64.dir/build.make bench/CMakeFiles/ListBenchSSB64.dir/ListBench.cpp.o.provides.build
.PHONY : bench/CMakeFiles/ListBenchSSB64.dir/ListBench.cpp.o.provides

bench/CMakeFiles/ListBenchSSB64.dir/ListBench.cpp.o.provides.build: bench/CMakeFiles/ListBenchSSB64.dir/ListBench.cpp.o


# Object files for target ListBenchSSB64
ListBenchSSB64_OBJECTS = \
"CMakeFiles/ListBenchSSB64.dir/ListBench.cpp.o"

# External object files for target ListBenchSSB64
ListBenchSSB64_EXTERNAL_OBJECTS =

bench/ListBenchSSB64: bench/CMakeFiles/ListBenchSSB64.dir/ListBench.cpp.o
bench/ListBenchSSB64: bench/CMakeFiles/ListBenchSSB64.dir/build.make
bench/ListBenchSSB64: libstm/libstm64.a
bench/ListBenchSSB64: bench/CMakeFiles/ListBenchSSB64.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/frantzcito/Desktop/rstm/rstm_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ListBenchSSB64"
	cd /home/frantzcito/Desktop/rstm/rstm_build/bench && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ListBenchSSB64.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
bench/CMakeFiles/ListBenchSSB64.dir/build: bench/ListBenchSSB64

.PHONY : bench/CMakeFiles/ListBenchSSB64.dir/build

bench/CMakeFiles/ListBenchSSB64.dir/requires: bench/CMakeFiles/ListBenchSSB64.dir/ListBench.cpp.o.requires

.PHONY : bench/CMakeFiles/ListBenchSSB64.dir/requires

bench/CMakeFiles/ListBenchSSB64.dir/clean:
	cd /home/frantzcito/Desktop/rstm/rstm_build/bench && $(CMAKE_COMMAND) -P CMakeFiles/ListBenchSSB64.dir/cmake_clean.cmake
.PHONY : bench/CMakeFiles/ListBenchSSB64.dir/clean

bench/CMakeFiles/ListBenchSSB64.dir/depend:
	cd /home/frantzcito/Desktop/rstm/rstm_build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/frantzcito/Desktop/rstm /home/frantzcito/Desktop/rstm/bench /home/frantzcito/Desktop/rstm/rstm_build /home/frantzcito/Desktop/rstm/rstm_build/bench /home/frantzcito/Desktop/rstm/rstm_build/bench/CMakeFiles/ListBenchSSB64.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : bench/CMakeFiles/ListBenchSSB64.dir/depend

