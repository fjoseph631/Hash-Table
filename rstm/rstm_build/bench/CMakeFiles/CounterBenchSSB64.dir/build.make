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
include bench/CMakeFiles/CounterBenchSSB64.dir/depend.make

# Include the progress variables for this target.
include bench/CMakeFiles/CounterBenchSSB64.dir/progress.make

# Include the compile flags for this target's objects.
include bench/CMakeFiles/CounterBenchSSB64.dir/flags.make

bench/CMakeFiles/CounterBenchSSB64.dir/CounterBench.cpp.o: bench/CMakeFiles/CounterBenchSSB64.dir/flags.make
bench/CMakeFiles/CounterBenchSSB64.dir/CounterBench.cpp.o: ../bench/CounterBench.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/frantzcito/Desktop/rstm/rstm_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object bench/CMakeFiles/CounterBenchSSB64.dir/CounterBench.cpp.o"
	cd /home/frantzcito/Desktop/rstm/rstm_build/bench && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/CounterBenchSSB64.dir/CounterBench.cpp.o -c /home/frantzcito/Desktop/rstm/bench/CounterBench.cpp

bench/CMakeFiles/CounterBenchSSB64.dir/CounterBench.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CounterBenchSSB64.dir/CounterBench.cpp.i"
	cd /home/frantzcito/Desktop/rstm/rstm_build/bench && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/frantzcito/Desktop/rstm/bench/CounterBench.cpp > CMakeFiles/CounterBenchSSB64.dir/CounterBench.cpp.i

bench/CMakeFiles/CounterBenchSSB64.dir/CounterBench.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CounterBenchSSB64.dir/CounterBench.cpp.s"
	cd /home/frantzcito/Desktop/rstm/rstm_build/bench && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/frantzcito/Desktop/rstm/bench/CounterBench.cpp -o CMakeFiles/CounterBenchSSB64.dir/CounterBench.cpp.s

bench/CMakeFiles/CounterBenchSSB64.dir/CounterBench.cpp.o.requires:

.PHONY : bench/CMakeFiles/CounterBenchSSB64.dir/CounterBench.cpp.o.requires

bench/CMakeFiles/CounterBenchSSB64.dir/CounterBench.cpp.o.provides: bench/CMakeFiles/CounterBenchSSB64.dir/CounterBench.cpp.o.requires
	$(MAKE) -f bench/CMakeFiles/CounterBenchSSB64.dir/build.make bench/CMakeFiles/CounterBenchSSB64.dir/CounterBench.cpp.o.provides.build
.PHONY : bench/CMakeFiles/CounterBenchSSB64.dir/CounterBench.cpp.o.provides

bench/CMakeFiles/CounterBenchSSB64.dir/CounterBench.cpp.o.provides.build: bench/CMakeFiles/CounterBenchSSB64.dir/CounterBench.cpp.o


# Object files for target CounterBenchSSB64
CounterBenchSSB64_OBJECTS = \
"CMakeFiles/CounterBenchSSB64.dir/CounterBench.cpp.o"

# External object files for target CounterBenchSSB64
CounterBenchSSB64_EXTERNAL_OBJECTS =

bench/CounterBenchSSB64: bench/CMakeFiles/CounterBenchSSB64.dir/CounterBench.cpp.o
bench/CounterBenchSSB64: bench/CMakeFiles/CounterBenchSSB64.dir/build.make
bench/CounterBenchSSB64: libstm/libstm64.a
bench/CounterBenchSSB64: bench/CMakeFiles/CounterBenchSSB64.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/frantzcito/Desktop/rstm/rstm_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable CounterBenchSSB64"
	cd /home/frantzcito/Desktop/rstm/rstm_build/bench && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/CounterBenchSSB64.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
bench/CMakeFiles/CounterBenchSSB64.dir/build: bench/CounterBenchSSB64

.PHONY : bench/CMakeFiles/CounterBenchSSB64.dir/build

bench/CMakeFiles/CounterBenchSSB64.dir/requires: bench/CMakeFiles/CounterBenchSSB64.dir/CounterBench.cpp.o.requires

.PHONY : bench/CMakeFiles/CounterBenchSSB64.dir/requires

bench/CMakeFiles/CounterBenchSSB64.dir/clean:
	cd /home/frantzcito/Desktop/rstm/rstm_build/bench && $(CMAKE_COMMAND) -P CMakeFiles/CounterBenchSSB64.dir/cmake_clean.cmake
.PHONY : bench/CMakeFiles/CounterBenchSSB64.dir/clean

bench/CMakeFiles/CounterBenchSSB64.dir/depend:
	cd /home/frantzcito/Desktop/rstm/rstm_build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/frantzcito/Desktop/rstm /home/frantzcito/Desktop/rstm/bench /home/frantzcito/Desktop/rstm/rstm_build /home/frantzcito/Desktop/rstm/rstm_build/bench /home/frantzcito/Desktop/rstm/rstm_build/bench/CMakeFiles/CounterBenchSSB64.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : bench/CMakeFiles/CounterBenchSSB64.dir/depend

