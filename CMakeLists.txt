cmake_minimum_required(VERSION 3.10)

# set the project name
project(RaceTrack VERSION 1.0)
message(STATUS "Compiling Race Track : ${PROJECT_NAME}")
set (CMAKE_CXX_STANDARD 17)
set (EXE RaceTrack)

set (CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

# Add necessary folder paths
include_directories(${PROJECT_SOURCE_DIR})
include_directories(lib/include)
include_directories(lib/vendor_includes)
include_directories(include)
include_directories(models/header)

file(GLOB LIB_SOURCES "lib/src/*.cpp")
file(GLOB LIB_VENDOR_SOURCES "lib/vendor_src/*.cpp")
file(GLOB PROJECT_SOURCE "src/*.cpp")
file(GLOB MAIN_SOURCE "Application.cpp")
file(GLOB MODEL_SOURCE "models/src/*.cpp")

add_executable(${EXE} ${LIB_SOURCES} ${LIB_VENDOR_SOURCES} ${PROJECT_SOURCE} ${MAIN_SOURCE} ${MODEL_SOURCE})

add_definitions(-Wfatal-errors -I/usr/local/include -I./)
set(BASIC_LIBRARIES "-L/usr/local/lib -lGL -lGLEW -lglfw")

target_link_libraries(${EXE} 
                      ${BASIC_LIBRARIES})

add_custom_target(
    clean_all 
    COMMAND rm -rf CMakeFiles
    COMMAND rm -rf build
    COMMAND rm -f cmake_install.cmake
    COMMAND rm -f CMakeCache.txt
    COMMENT "Cleaning make and cmake created files"
)

add_custom_target(
    run 
    COMMAND make all
    COMMAND ./build/bin/${PROJECT_NAME}
    COMMAND make clean_all
    COMMENT "Running all files"
)