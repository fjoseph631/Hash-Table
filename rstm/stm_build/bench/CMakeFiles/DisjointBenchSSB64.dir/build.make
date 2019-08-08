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
CMAKE_SOURCE_DIR = "/home/frantzcito/Desktop/Final Project/rstm"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/frantzcito/Desktop/Final Project/rstm/stm_build"

# Include any dependencies generated for this target.
include bench/CMakeFiles/DisjointBenchSSB64.dir/depend.make

# Include the progress variables for this target.
include bench/CMakeFiles/DisjointBenchSSB64.dir/progress.make

# Include the compile flags for this target's objects.
include bench/CMakeFiles/DisjointBenchSSB64.dir/flags.make

bench/CMakeFiles/DisjointBenchSSB64.dir/DisjointBench.cpp.o: bench/CMakeFiles/DisjointBenchSSB64.dir/flags.make
bench/CMakeFiles/DisjointBenchSSB64.dir/DisjointBench.cpp.o: ../bench/DisjointBench.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/frantzcito/Desktop/Final Project/rstm/stm_build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object bench/CMakeFiles/DisjointBenchSSB64.dir/DisjointBench.cpp.o"
	cd "/home/frantzcito/Desktop/Final Project/rstm/stm_build/bench" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/DisjointBenchSSB64.dir/DisjointBench.cpp.o -c "/home/frantzcito/Desktop/Final Project/rstm/bench/DisjointBench.cpp"

bench/CMakeFiles/DisjointBenchSSB64.dir/DisjointBench.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/DisjointBenchSSB64.dir/DisjointBench.cpp.i"
	cd "/home/frantzcito/Desktop/Final Project/rstm/stm_build/bench" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/frantzcito/Desktop/Final Project/rstm/bench/DisjointBench.cpp" > CMakeFiles/DisjointBenchSSB64.dir/DisjointBench.cpp.i

bench/CMakeFiles/DisjointBenchSSB64.dir/DisjointBench.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/DisjointBenchSSB64.dir/DisjointBench.cpp.s"
	cd "/home/frantzcito/Desktop/Final Project/rstm/stm_build/bench" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/frantzcito/Desktop/Final Project/rstm/bench/DisjointBench.cpp" -o CMakeFiles/DisjointBenchSSB64.dir/DisjointBench.cpp.s

bench/CMakeFiles/DisjointBenchSSB64.dir/DisjointBench.cpp.o.requires:

.PHONY : bench/CMakeFiles/DisjointBenchSSB64.dir/DisjointBench.cpp.o.requires

bench/CMakeFiles/DisjointBenchSSB64.dir/DisjointBench.cpp.o.provides: bench/CMakeFiles/DisjointBenchSSB64.dir/DisjointBench.cpp.o.requires
	$(MAKE) -f bench/CMakeFiles/DisjointBenchSSB64.dir/build.make bench/CMakeFiles/DisjointBenchSSB64.dir/DisjointBench.cpp.o.provides.build
.PHONY : bench/CMakeFiles/DisjointBenchSSB64.dir/DisjointBench.cpp.o.provides

bench/CMakeFiles/DisjointBenchSSB64.dir/DisjointBench.cpp.o.provides.build: bench/CMakeFiles/DisjointBenchSSB64.dir/DisjointBench.cpp.o


# Object files for target DisjointBenchSSB64
DisjointBenchSSB64_OBJECTS = \
"CMakeFiles/DisjointBenchSSB64.dir/DisjointBench.cpp.o"

# External object files for target DisjointBenchSSB64
DisjointBenchSSB64_EXTERNAL_OBJECTS =

bench/DisjointBenchSSB64: bench/CMakeFiles/DisjointBenchSSB64.dir/DisjointBench.cpp.o
bench/DisjointBenchSSB64: bench/CMakeFiles/DisjointBenchSSB64.dir/build.make
bench/DisjointBenchSSB64: libstm/libstm64.a
bench/DisjointBenchSSB64: bench/CMakeFiles/DisjointBenchSSB64.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/frantzcito/Desktop/Final Project/rstm/stm_build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable DisjointBenchSSB64"
	cd "/home/frantzcito/Desktop/Final Project/rstm/stm_build/bench" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/DisjointBenchSSB64.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
bench/CMakeFiles/DisjointBenchSSB64.dir/build: bench/DisjointBenchSSB64

.PHONY : bench/CMakeFiles/DisjointBenchSSB64.dir/build

bench/CMakeFiles/DisjointBenchSSB64.dir/requires: bench/CMakeFiles/DisjointBenchSSB64.dir/DisjointBench.cpp.o.requires

.PHONY : bench/CMakeFiles/DisjointBenchSSB64.dir/requires

bench/CMakeFiles/DisjointBenchSSB64.dir/clean:
	cd "/home/frantzcito/Desktop/Final Project/rstm/stm_build/bench" && $(CMAKE_COMMAND) -P CMakeFiles/DisjointBenchSSB64.dir/cmake_clean.cmake
.PHONY : bench/CMakeFiles/DisjointBenchSSB64.dir/clean

bench/CMakeFiles/DisjointBenchSSB64.dir/depend:
	cd "/home/frantzcito/Desktop/Final Project/rstm/stm_build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/frantzcito/Desktop/Final Project/rstm" "/home/frantzcito/Desktop/Final Project/rstm/bench" "/home/frantzcito/Desktop/Final Project/rstm/stm_build" "/home/frantzcito/Desktop/Final Project/rstm/stm_build/bench" "/home/frantzcito/Desktop/Final Project/rstm/stm_build/bench/CMakeFiles/DisjointBenchSSB64.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : bench/CMakeFiles/DisjointBenchSSB64.dir/depend

