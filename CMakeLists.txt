cmake_minimum_required(VERSION 3.10)

project(LeetCode)

# Глобальні налаштування
# Директорій для збірки
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
# Сандарт
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# ==================================================================

set(dubug_executable_name "curr")
add_subdirectory(source/_aux)

# ==================================================================

function(set_debug_output_name target_name)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set_target_properties(${target_name} PROPERTIES OUTPUT_NAME ${dubug_executable_name})
    endif()
endfunction()

set(catch_path "D:/Programming/Catch2")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(catch_lib_path "${catch_path}/build_mk0/debug/src/Debug")
    set(catch_include_path "${catch_path}/build_mk0/debug/generated-includes")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(catch_lib_path "${catch_path}/build_mk0/release/src/Debug")
    set(catch_include_path "${catch_path}/build_mk0/release/generated-includes")
endif()

add_subdirectory("${catch_path}" "${catch_path}/build" EXCLUDE_FROM_ALL)

find_package(Catch2 3 REQUIRED)


# =================   addTwoNumbers  ===============================

add_executable(addTwoNumbers source/addTwoNumbers/main.cpp)
target_link_libraries(addTwoNumbers PRIVATE _aux)
set_debug_output_name(addTwoNumbers)


# =================  Factorial with Catch2 =========================


add_executable(test_factorial source/factorial/test_factorial.cpp source/factorial/factorial.cpp)
target_link_libraries(test_factorial PRIVATE Catch2::Catch2WithMain)

# ===================  CASH ========================================

add_executable(cash source/cash/main.cpp)
target_link_libraries(cash PRIVATE _aux)
set_debug_output_name(cash)


# ==================================================================
