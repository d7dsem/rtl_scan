# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

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
CMAKE_SOURCE_DIR = /home/d7/Public/rtl_scan

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/d7/Public/rtl_scan/build/tests

# Include any dependencies generated for this target.
include CMakeFiles/rtls.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/rtls.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/rtls.dir/flags.make

CMakeFiles/rtls.dir/source/main/main.cpp.o: CMakeFiles/rtls.dir/flags.make
CMakeFiles/rtls.dir/source/main/main.cpp.o: ../../source/main/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/d7/Public/rtl_scan/build/tests/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/rtls.dir/source/main/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/rtls.dir/source/main/main.cpp.o -c /home/d7/Public/rtl_scan/source/main/main.cpp

CMakeFiles/rtls.dir/source/main/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rtls.dir/source/main/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/d7/Public/rtl_scan/source/main/main.cpp > CMakeFiles/rtls.dir/source/main/main.cpp.i

CMakeFiles/rtls.dir/source/main/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rtls.dir/source/main/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/d7/Public/rtl_scan/source/main/main.cpp -o CMakeFiles/rtls.dir/source/main/main.cpp.s

CMakeFiles/rtls.dir/source/main/demo_stuff.cpp.o: CMakeFiles/rtls.dir/flags.make
CMakeFiles/rtls.dir/source/main/demo_stuff.cpp.o: ../../source/main/demo_stuff.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/d7/Public/rtl_scan/build/tests/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/rtls.dir/source/main/demo_stuff.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/rtls.dir/source/main/demo_stuff.cpp.o -c /home/d7/Public/rtl_scan/source/main/demo_stuff.cpp

CMakeFiles/rtls.dir/source/main/demo_stuff.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rtls.dir/source/main/demo_stuff.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/d7/Public/rtl_scan/source/main/demo_stuff.cpp > CMakeFiles/rtls.dir/source/main/demo_stuff.cpp.i

CMakeFiles/rtls.dir/source/main/demo_stuff.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rtls.dir/source/main/demo_stuff.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/d7/Public/rtl_scan/source/main/demo_stuff.cpp -o CMakeFiles/rtls.dir/source/main/demo_stuff.cpp.s

# Object files for target rtls
rtls_OBJECTS = \
"CMakeFiles/rtls.dir/source/main/main.cpp.o" \
"CMakeFiles/rtls.dir/source/main/demo_stuff.cpp.o"

# External object files for target rtls
rtls_EXTERNAL_OBJECTS =

bin/curr: CMakeFiles/rtls.dir/source/main/main.cpp.o
bin/curr: CMakeFiles/rtls.dir/source/main/demo_stuff.cpp.o
bin/curr: CMakeFiles/rtls.dir/build.make
bin/curr: lib/lib_aux.a
bin/curr: lib/lib_aux.a
bin/curr: /usr/lib/x86_64-linux-gnu/libfftw3.so
bin/curr: /usr/lib/x86_64-linux-gnu/librtlsdr.so
bin/curr: CMakeFiles/rtls.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/d7/Public/rtl_scan/build/tests/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable bin/curr"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/rtls.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/rtls.dir/build: bin/curr

.PHONY : CMakeFiles/rtls.dir/build

CMakeFiles/rtls.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/rtls.dir/cmake_clean.cmake
.PHONY : CMakeFiles/rtls.dir/clean

CMakeFiles/rtls.dir/depend:
	cd /home/d7/Public/rtl_scan/build/tests && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/d7/Public/rtl_scan /home/d7/Public/rtl_scan /home/d7/Public/rtl_scan/build/tests /home/d7/Public/rtl_scan/build/tests /home/d7/Public/rtl_scan/build/tests/CMakeFiles/rtls.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/rtls.dir/depend
