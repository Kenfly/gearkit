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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/kenfly/gearkit/test

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kenfly/gearkit/test/client

# Include any dependencies generated for this target.
include out/CMakeFiles/gearkit.dir/depend.make

# Include the progress variables for this target.
include out/CMakeFiles/gearkit.dir/progress.make

# Include the compile flags for this target's objects.
include out/CMakeFiles/gearkit.dir/flags.make

out/CMakeFiles/gearkit.dir/Application.cpp.o: out/CMakeFiles/gearkit.dir/flags.make
out/CMakeFiles/gearkit.dir/Application.cpp.o: ../../Application.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kenfly/gearkit/test/client/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object out/CMakeFiles/gearkit.dir/Application.cpp.o"
	cd /home/kenfly/gearkit/test/client/out && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/gearkit.dir/Application.cpp.o -c /home/kenfly/gearkit/Application.cpp

out/CMakeFiles/gearkit.dir/Application.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gearkit.dir/Application.cpp.i"
	cd /home/kenfly/gearkit/test/client/out && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kenfly/gearkit/Application.cpp > CMakeFiles/gearkit.dir/Application.cpp.i

out/CMakeFiles/gearkit.dir/Application.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gearkit.dir/Application.cpp.s"
	cd /home/kenfly/gearkit/test/client/out && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kenfly/gearkit/Application.cpp -o CMakeFiles/gearkit.dir/Application.cpp.s

out/CMakeFiles/gearkit.dir/Application.cpp.o.requires:

.PHONY : out/CMakeFiles/gearkit.dir/Application.cpp.o.requires

out/CMakeFiles/gearkit.dir/Application.cpp.o.provides: out/CMakeFiles/gearkit.dir/Application.cpp.o.requires
	$(MAKE) -f out/CMakeFiles/gearkit.dir/build.make out/CMakeFiles/gearkit.dir/Application.cpp.o.provides.build
.PHONY : out/CMakeFiles/gearkit.dir/Application.cpp.o.provides

out/CMakeFiles/gearkit.dir/Application.cpp.o.provides.build: out/CMakeFiles/gearkit.dir/Application.cpp.o


# Object files for target gearkit
gearkit_OBJECTS = \
"CMakeFiles/gearkit.dir/Application.cpp.o"

# External object files for target gearkit
gearkit_EXTERNAL_OBJECTS =

out/libgearkit.a: out/CMakeFiles/gearkit.dir/Application.cpp.o
out/libgearkit.a: out/CMakeFiles/gearkit.dir/build.make
out/libgearkit.a: out/CMakeFiles/gearkit.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kenfly/gearkit/test/client/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libgearkit.a"
	cd /home/kenfly/gearkit/test/client/out && $(CMAKE_COMMAND) -P CMakeFiles/gearkit.dir/cmake_clean_target.cmake
	cd /home/kenfly/gearkit/test/client/out && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gearkit.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
out/CMakeFiles/gearkit.dir/build: out/libgearkit.a

.PHONY : out/CMakeFiles/gearkit.dir/build

out/CMakeFiles/gearkit.dir/requires: out/CMakeFiles/gearkit.dir/Application.cpp.o.requires

.PHONY : out/CMakeFiles/gearkit.dir/requires

out/CMakeFiles/gearkit.dir/clean:
	cd /home/kenfly/gearkit/test/client/out && $(CMAKE_COMMAND) -P CMakeFiles/gearkit.dir/cmake_clean.cmake
.PHONY : out/CMakeFiles/gearkit.dir/clean

out/CMakeFiles/gearkit.dir/depend:
	cd /home/kenfly/gearkit/test/client && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kenfly/gearkit/test /home/kenfly/gearkit /home/kenfly/gearkit/test/client /home/kenfly/gearkit/test/client/out /home/kenfly/gearkit/test/client/out/CMakeFiles/gearkit.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : out/CMakeFiles/gearkit.dir/depend
