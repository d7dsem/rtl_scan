cmake_minimum_required(VERSION 3.10)

project(RrlScan)

# Глобальні налаштування
# Директорій для збірки
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
# Сандарт
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# ==================================================================
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(_DEBUG)
endif()

function(set_debug_output_name target_name)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set_target_properties(${target_name} PROPERTIES OUTPUT_NAME ${dubug_executable_name})
    endif()
endfunction()

set(dubug_executable_name "curr")
add_subdirectory(source/_aux)

# ===================  MAIN ========================================

add_executable(rtls source/main/main.cpp source/main/demo_stuff.cpp)

# Inproject dependencies

target_include_directories(rtls PRIVATE source/cpp-httplib)
target_link_libraries(rtls PRIVATE _aux)
set_debug_output_name(rtls)

# External dependencies

find_package(Threads REQUIRED)
target_link_libraries(rtls PRIVATE _aux Threads::Threads)

find_library(FFTW_LIB fftw3 REQUIRED PATHS /usr/lib/x86_64-linux-gnu)
target_include_directories(rtls PRIVATE /usr/include)
target_link_libraries(rtls PRIVATE ${FFTW_LIB})

find_library(RTLSDR_LIB rtlsdr REQUIRED PATHS /usr/lib/x86_64-linux-gnu)
target_include_directories(rtls PRIVATE /usr/include)
target_link_libraries(rtls PRIVATE ${RTLSDR_LIB})
# Optimization flags
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(rtls PRIVATE -O2)
endif()


# ===================  TESTS =======================================
find_package(Catch2 REQUIRED)
add_executable(tests_rtl test/test_rtl_stuff.cpp source/main/rtl_sdr_stuff.cpp)
target_include_directories(tests_rtl PRIVATE /usr/include/catch2)
target_link_libraries(tests_rtl PRIVATE Catch2::Catch2WithMain)
target_link_libraries(tests_rtl PRIVATE ${RTLSDR_LIB})
target_compile_features(tests_rtl PRIVATE cxx_std_17)
# ==================================================================
