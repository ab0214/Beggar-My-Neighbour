cmake_minimum_required(VERSION 3.14)
project(Beggar-My-Neighbour)
set(CMAKE_CXX_STANDARD 17)

# For testing: Download and build GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0 
)
# For Windows: Prevent googletest from overriding compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
# Enable testing
enable_testing()

# Add include directories (header files) to the build
include_directories(${PROJECT_SOURCE_DIR}/include)
# Add source files to a new variable named SOURCES
file(GLOB_RECURSE SOURCES "src/*.cpp")
# Add main executable to the project using source files in SOURCES
add_executable(${PROJECT_NAME} ${SOURCES})

# Remove main.cpp from SOURCES for building the test executable
# to avoid multiple entry points in the test executable
list(REMOVE_ITEM SOURCES "${PROJECT_SOURCE_DIR}/src/main.cpp")
message(STATUS "SOURCES: ${SOURCES}")
# Add test files to new variable named TEST_SOURCES
file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
# Add test executable using both TEST_SOURCES and SOURCES
add_executable(${PROJECT_NAME}-tests ${TEST_SOURCES} ${SOURCES})
# Link test executable against gtest and gtest_main libraries
target_link_libraries(${PROJECT_NAME}-tests gtest gtest_main)

# Automatically add tests by querying the compiled test executable
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}-tests)