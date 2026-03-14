cmake_minimum_required(VERSION 3.23)

project(aZeroMediumAudio)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SDL_SHARED OFF)
set(SDL_STATIC ON)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")

option(TEST_EXE "Make it as an exe instead of lib" OFF)

if (TEST_EXE)
    add_executable(aZeroMediumAudio
        "src/MediumAudio.cpp" 
        "examples/main.cpp"
)
else()
    add_library(aZeroMediumAudio 
        "src/MediumAudio.cpp")
endif()

if (USE_DEBUG)
    target_compile_definitions(aZeroMediumAudio PUBLIC USE_DEBUG)
endif()

include(FetchContent)

# miniaudio
FetchContent_Declare(
    miniaudio
    GIT_REPOSITORY https://github.com/mackron/miniaudio.git
    GIT_TAG        "master"
)
FetchContent_MakeAvailable(miniaudio)
#

target_include_directories(aZeroMediumAudio
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_BINARY_DIR}/_deps/miniaudio-src
)

target_link_libraries(aZeroMediumAudio PUBLIC miniaudio)