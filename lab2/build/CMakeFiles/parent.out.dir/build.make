# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = "/home/dmitry/Рабочий стол/OC/lab2/build"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/dmitry/Рабочий стол/OC/lab2/build"

# Include any dependencies generated for this target.
include CMakeFiles/parent.out.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/parent.out.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/parent.out.dir/flags.make

CMakeFiles/parent.out.dir/home/dmitry/Рабочий_стол/OC/lab2/src/parent.cpp.o: CMakeFiles/parent.out.dir/flags.make
CMakeFiles/parent.out.dir/home/dmitry/Рабочий_стол/OC/lab2/src/parent.cpp.o: /home/dmitry/Рабочий\ стол/OC/lab2/src/parent.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/dmitry/Рабочий стол/OC/lab2/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/parent.out.dir/home/dmitry/Рабочий_стол/OC/lab2/src/parent.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/parent.out.dir/home/dmitry/Рабочий_стол/OC/lab2/src/parent.cpp.o -c "/home/dmitry/Рабочий стол/OC/lab2/src/parent.cpp"

CMakeFiles/parent.out.dir/home/dmitry/Рабочий_стол/OC/lab2/src/parent.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/parent.out.dir/home/dmitry/Рабочий_стол/OC/lab2/src/parent.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/dmitry/Рабочий стол/OC/lab2/src/parent.cpp" > CMakeFiles/parent.out.dir/home/dmitry/Рабочий_стол/OC/lab2/src/parent.cpp.i

CMakeFiles/parent.out.dir/home/dmitry/Рабочий_стол/OC/lab2/src/parent.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/parent.out.dir/home/dmitry/Рабочий_стол/OC/lab2/src/parent.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/dmitry/Рабочий стол/OC/lab2/src/parent.cpp" -o CMakeFiles/parent.out.dir/home/dmitry/Рабочий_стол/OC/lab2/src/parent.cpp.s

# Object files for target parent.out
parent_out_OBJECTS = \
"CMakeFiles/parent.out.dir/home/dmitry/Рабочий_стол/OC/lab2/src/parent.cpp.o"

# External object files for target parent.out
parent_out_EXTERNAL_OBJECTS =

parent.out: CMakeFiles/parent.out.dir/home/dmitry/Рабочий_стол/OC/lab2/src/parent.cpp.o
parent.out: CMakeFiles/parent.out.dir/build.make
parent.out: CMakeFiles/parent.out.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/dmitry/Рабочий стол/OC/lab2/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable parent.out"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/parent.out.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/parent.out.dir/build: parent.out

.PHONY : CMakeFiles/parent.out.dir/build

CMakeFiles/parent.out.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/parent.out.dir/cmake_clean.cmake
.PHONY : CMakeFiles/parent.out.dir/clean

CMakeFiles/parent.out.dir/depend:
	cd "/home/dmitry/Рабочий стол/OC/lab2/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/dmitry/Рабочий стол/OC/lab2/build" "/home/dmitry/Рабочий стол/OC/lab2/build" "/home/dmitry/Рабочий стол/OC/lab2/build" "/home/dmitry/Рабочий стол/OC/lab2/build" "/home/dmitry/Рабочий стол/OC/lab2/build/CMakeFiles/parent.out.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/parent.out.dir/depend

