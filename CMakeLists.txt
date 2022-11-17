cmake_minimum_required(VERSION 3.0.0)
project(Zipper VERSION 0.1.0)

include(CTest)
enable_testing()

# Set source directory
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Set include directory
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Set resource directory
set(RESOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources)

# Set project files
FILE(GLOB SOURCE_FILES ${SOURCE_DIR}/*.c)
FILE(GLOB INCLUDE_FILES ${INCLUDE_DIR}/*.h)
FILE(GLOB RESOURCE_FILES ${RESOURCE_DIR}/*)

# make include files available to the compiler
include_directories(${INCLUDE_DIR})
# make resources available to the compiler
include_directories(${RESOURCE_DIR})

# Set executable
add_executable(Zipper ${SOURCE_FILES} ${INCLUDE_FILES} ${RESOURCE_FILES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

