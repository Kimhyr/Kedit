cmake_minimum_required(VERSION 3.14)
set(KEDIT_VERSION 1.0)
set(KEDIT_LANGUAGE cpp)

# set(EXECUTABLE_OUTPUT_PATH ./Build)

project(KEDIT)

file(GLOB KEDIT_FILES "./Source/*.cpp")
set(KEDIT_SOURCES ${KEDIT_FILES})
add_executable(
    KEDIT
    ${KEDIT_SOURCES}
)

target_include_directories(
    KEDIT PRIVATE ./Source
)
