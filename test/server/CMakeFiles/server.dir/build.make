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
CMAKE_SOURCE_DIR = /home/kenfly/myspace/gearkit/test

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kenfly/myspace/gearkit/test/server

# Include any dependencies generated for this target.
include CMakeFiles/server.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/server.dir/flags.make

CMakeFiles/server.dir/app_server.cpp.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/app_server.cpp.o: ../app_server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kenfly/myspace/gearkit/test/server/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/server.dir/app_server.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/server.dir/app_server.cpp.o -c /home/kenfly/myspace/gearkit/test/app_server.cpp

CMakeFiles/server.dir/app_server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/app_server.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kenfly/myspace/gearkit/test/app_server.cpp > CMakeFiles/server.dir/app_server.cpp.i

CMakeFiles/server.dir/app_server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/app_server.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kenfly/myspace/gearkit/test/app_server.cpp -o CMakeFiles/server.dir/app_server.cpp.s

CMakeFiles/server.dir/app_server.cpp.o.requires:

.PHONY : CMakeFiles/server.dir/app_server.cpp.o.requires

CMakeFiles/server.dir/app_server.cpp.o.provides: CMakeFiles/server.dir/app_server.cpp.o.requires
	$(MAKE) -f CMakeFiles/server.dir/build.make CMakeFiles/server.dir/app_server.cpp.o.provides.build
.PHONY : CMakeFiles/server.dir/app_server.cpp.o.provides

CMakeFiles/server.dir/app_server.cpp.o.provides.build: CMakeFiles/server.dir/app_server.cpp.o


CMakeFiles/server.dir/main_server.cpp.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/main_server.cpp.o: ../main_server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kenfly/myspace/gearkit/test/server/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/server.dir/main_server.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/server.dir/main_server.cpp.o -c /home/kenfly/myspace/gearkit/test/main_server.cpp

CMakeFiles/server.dir/main_server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/main_server.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kenfly/myspace/gearkit/test/main_server.cpp > CMakeFiles/server.dir/main_server.cpp.i

CMakeFiles/server.dir/main_server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/main_server.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kenfly/myspace/gearkit/test/main_server.cpp -o CMakeFiles/server.dir/main_server.cpp.s

CMakeFiles/server.dir/main_server.cpp.o.requires:

.PHONY : CMakeFiles/server.dir/main_server.cpp.o.requires

CMakeFiles/server.dir/main_server.cpp.o.provides: CMakeFiles/server.dir/main_server.cpp.o.requires
	$(MAKE) -f CMakeFiles/server.dir/build.make CMakeFiles/server.dir/main_server.cpp.o.provides.build
.PHONY : CMakeFiles/server.dir/main_server.cpp.o.provides

CMakeFiles/server.dir/main_server.cpp.o.provides.build: CMakeFiles/server.dir/main_server.cpp.o


# Object files for target server
server_OBJECTS = \
"CMakeFiles/server.dir/app_server.cpp.o" \
"CMakeFiles/server.dir/main_server.cpp.o"

# External object files for target server
server_EXTERNAL_OBJECTS =

server: CMakeFiles/server.dir/app_server.cpp.o
server: CMakeFiles/server.dir/main_server.cpp.o
server: CMakeFiles/server.dir/build.make
server: out/libgearkit.a
server: out/network/libnetwork.a
server: out/kit/libkit.a
server: CMakeFiles/server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kenfly/myspace/gearkit/test/server/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable server"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/server.dir/build: server

.PHONY : CMakeFiles/server.dir/build

CMakeFiles/server.dir/requires: CMakeFiles/server.dir/app_server.cpp.o.requires
CMakeFiles/server.dir/requires: CMakeFiles/server.dir/main_server.cpp.o.requires

.PHONY : CMakeFiles/server.dir/requires

CMakeFiles/server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/server.dir/clean

CMakeFiles/server.dir/depend:
	cd /home/kenfly/myspace/gearkit/test/server && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kenfly/myspace/gearkit/test /home/kenfly/myspace/gearkit/test /home/kenfly/myspace/gearkit/test/server /home/kenfly/myspace/gearkit/test/server /home/kenfly/myspace/gearkit/test/server/CMakeFiles/server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/server.dir/depend

