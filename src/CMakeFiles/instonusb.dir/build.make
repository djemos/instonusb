# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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
CMAKE_SOURCE_DIR = /home/djemos/instonusb

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/djemos/instonusb

# Include any dependencies generated for this target.
include src/CMakeFiles/instonusb.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/instonusb.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/instonusb.dir/flags.make

src/CMakeFiles/instonusb.dir/instonusb.o: src/CMakeFiles/instonusb.dir/flags.make
src/CMakeFiles/instonusb.dir/instonusb.o: src/instonusb.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/djemos/instonusb/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/CMakeFiles/instonusb.dir/instonusb.o"
	cd /home/djemos/instonusb/src && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/instonusb.dir/instonusb.o   -c /home/djemos/instonusb/src/instonusb.c

src/CMakeFiles/instonusb.dir/instonusb.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/instonusb.dir/instonusb.i"
	cd /home/djemos/instonusb/src && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/djemos/instonusb/src/instonusb.c > CMakeFiles/instonusb.dir/instonusb.i

src/CMakeFiles/instonusb.dir/instonusb.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/instonusb.dir/instonusb.s"
	cd /home/djemos/instonusb/src && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/djemos/instonusb/src/instonusb.c -o CMakeFiles/instonusb.dir/instonusb.s

# Object files for target instonusb
instonusb_OBJECTS = \
"CMakeFiles/instonusb.dir/instonusb.o"

# External object files for target instonusb
instonusb_EXTERNAL_OBJECTS =

src/instonusb: src/CMakeFiles/instonusb.dir/instonusb.o
src/instonusb: src/CMakeFiles/instonusb.dir/build.make
src/instonusb: src/CMakeFiles/instonusb.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/djemos/instonusb/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable instonusb"
	cd /home/djemos/instonusb/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/instonusb.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/instonusb.dir/build: src/instonusb

.PHONY : src/CMakeFiles/instonusb.dir/build

src/CMakeFiles/instonusb.dir/clean:
	cd /home/djemos/instonusb/src && $(CMAKE_COMMAND) -P CMakeFiles/instonusb.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/instonusb.dir/clean

src/CMakeFiles/instonusb.dir/depend:
	cd /home/djemos/instonusb && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/djemos/instonusb /home/djemos/instonusb/src /home/djemos/instonusb /home/djemos/instonusb/src /home/djemos/instonusb/src/CMakeFiles/instonusb.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/instonusb.dir/depend

