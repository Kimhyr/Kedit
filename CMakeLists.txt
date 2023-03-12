cmake_minimum_required(VERSION 3.14)
set(KEDIT_VERSION 1.0)
set(KEDIT_LANGUAGE cpp)

# set(EXECUTABLE_OUTPUT_PATH ./Build)

project(KEDIT)

set(KEDIT_PATH ./Source)
file(GLOB KEDIT_FILES ${KEDIT_PATH}/*.cpp)
file(GLOB KEDIT_CORE_FILES ${KEDIT_PATH}/Core/*.cpp)
file(GLOB KEDIT_UTILITY_FILES ${KEDIT_PATH}/Utility/*.cpp)
set(KEDIT_SOURCES ${KEDIT_FILES} ${KEDIT_CORE_FILES} ${KEDIT_UTILITY_FILES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Source)

add_executable(KEDIT ${KEDIT_SOURCES})
target_include_directories(KEDIT PRIVATE ${KEDIT_PATH})
