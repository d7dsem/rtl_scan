cmake_minimum_required(VERSION 3.10)

project(_aux)

set(SRC_FILES 
    d7.cpp  # Actual source file in _aux directory
)

set(HEADER_FILES
    d7.hpp  
)

# Create a static library from the source files
add_library(_aux STATIC ${SRC_FILES}  ${HEADER_FILES})

# Set C++ standard
set_target_properties(_aux PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES)
if(MSVC)
    target_compile_options(_aux PRIVATE /Zc:__cplusplus)
	target_compile_definitions(_aux PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
target_compile_features(_aux PUBLIC cxx_std_17)

# Include directory for aux library
target_include_directories(_aux PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Set output folder for lb
set_target_properties(_aux PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Uncoment for check
#message(STATUS "AUX CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
#message(STATUS "AUX CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")

# For future
#install(TARGETS _aux ARCHIVE DESTINATION ${CMAKE_BINARY_DIR}/bin RUNTIME DESTINATION ${CMAKE_BINARY_DIR}/bin)
#install(FILES d7.hpp DESTINATION ${CMAKE_BINARY_DIR}/bin)