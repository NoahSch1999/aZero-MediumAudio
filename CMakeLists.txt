cmake_minimum_required(VERSION 3.23)

project(aZeroAudio)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(COMPILE_EXAMPLE "Make it as an exe instead of lib" OFF)

include(FetchContent)

if (COMPILE_EXAMPLE)
    add_executable(aZeroAudio
        "src/aZeroAudio.cpp" 
        "examples/main.cpp"
    )
    add_compile_definitions(AUDIO_SOURCE_PATH="${CMAKE_SOURCE_DIR}/examples/" AUDIO_SOURCE_PATH)
    target_compile_definitions(aZeroAudio 
        PUBLIC 
        COMPILE_EXAMPLE
        AUDIO_SOURCE_PATH
    )
else()
    add_library(aZeroAudio 
        "src/aZeroAudio.cpp")
endif()

# miniaudio
FetchContent_Declare(
    miniaudio
    GIT_REPOSITORY https://github.com/mackron/miniaudio.git
    GIT_TAG        "master"
)
FetchContent_MakeAvailable(miniaudio)
#

target_include_directories(aZeroAudio
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_BINARY_DIR}/_deps/miniaudio-src
)

target_link_libraries(aZeroAudio PUBLIC miniaudio)