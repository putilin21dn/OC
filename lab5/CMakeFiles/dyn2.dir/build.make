# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/dmitry/Рабочий стол/OC/lab5"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/dmitry/Рабочий стол/OC/lab5"

# Include any dependencies generated for this target.
include CMakeFiles/dyn2.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/dyn2.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/dyn2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/dyn2.dir/flags.make

CMakeFiles/dyn2.dir/src/realisation2.c.o: CMakeFiles/dyn2.dir/flags.make
CMakeFiles/dyn2.dir/src/realisation2.c.o: src/realisation2.c
CMakeFiles/dyn2.dir/src/realisation2.c.o: CMakeFiles/dyn2.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/dmitry/Рабочий стол/OC/lab5/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/dyn2.dir/src/realisation2.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/dyn2.dir/src/realisation2.c.o -MF CMakeFiles/dyn2.dir/src/realisation2.c.o.d -o CMakeFiles/dyn2.dir/src/realisation2.c.o -c "/home/dmitry/Рабочий стол/OC/lab5/src/realisation2.c"

CMakeFiles/dyn2.dir/src/realisation2.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dyn2.dir/src/realisation2.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/home/dmitry/Рабочий стол/OC/lab5/src/realisation2.c" > CMakeFiles/dyn2.dir/src/realisation2.c.i

CMakeFiles/dyn2.dir/src/realisation2.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dyn2.dir/src/realisation2.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/home/dmitry/Рабочий стол/OC/lab5/src/realisation2.c" -o CMakeFiles/dyn2.dir/src/realisation2.c.s

# Object files for target dyn2
dyn2_OBJECTS = \
"CMakeFiles/dyn2.dir/src/realisation2.c.o"

# External object files for target dyn2
dyn2_EXTERNAL_OBJECTS =

libdyn2.so: CMakeFiles/dyn2.dir/src/realisation2.c.o
libdyn2.so: CMakeFiles/dyn2.dir/build.make
libdyn2.so: CMakeFiles/dyn2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/dmitry/Рабочий стол/OC/lab5/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking C shared library libdyn2.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/dyn2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/dyn2.dir/build: libdyn2.so
.PHONY : CMakeFiles/dyn2.dir/build

CMakeFiles/dyn2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/dyn2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/dyn2.dir/clean

CMakeFiles/dyn2.dir/depend:
	cd "/home/dmitry/Рабочий стол/OC/lab5" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/dmitry/Рабочий стол/OC/lab5" "/home/dmitry/Рабочий стол/OC/lab5" "/home/dmitry/Рабочий стол/OC/lab5" "/home/dmitry/Рабочий стол/OC/lab5" "/home/dmitry/Рабочий стол/OC/lab5/CMakeFiles/dyn2.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/dyn2.dir/depend
