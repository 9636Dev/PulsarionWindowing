cmake_minimum_required(VERSION 3.27)
project(PulsarionWindowing VERSION 0.0.1 DESCRIPTION "Pulsarion Windowing Library")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PULSARION_WINDOWING_SOURCES
    src/PulsarionWindowing/Core.hpp
    src/PulsarionWindowing/Window.hpp # Base window class
    src/PulsarionWindowing/WindowFlags.hpp
)

# If it is windows
if (WIN32)
    set(PULSARION_WINDOWING_PLATFORM_SPECIFIC_SOURCES
        src/PulsarionWindowing/Windows/Window.cpp
        src/PulsarionWindowing/Windows/Window.hpp
    )
elseif (DARWIN)
    set(PULSARION_WINDOWING_PLATFORM_SPECIFIC_SOURCES
        src/PulsarionWindowing/MacOS/Window.mm
        src/PulsarionWindowing/MacOS/Window.hpp
    )
endif()

message(STATUS "PULSARION_Windowing Sources: ${PULSARION_WINDOWING_PLATFORM_SPECIFIC_SOURCES}")

if (NOT DEFINED PULSARION_LIBRARY_TYPE)
    message(FATAL_ERROR "PULSARION_LIBRARY_TYPE is not defined, please include PulsarionCore first!")
    return()
endif()


if (NOT DEFINED PULSARION_RENDERER)
    message(FATAL_ERROR "PULSARION_RENDERER is not defined, please include PulsarionCore first!")
    return()
endif()

add_library(PulsarionWindowing ${PULSARION_LIBRARY_TYPE} ${PULSARION_WINDOWING_SOURCES} ${PULSARION_WINDOWING_PLATFORM_SPECIFIC_SOURCES})

if (PULSARION_LIBRARY_TYPE STREQUAL "SHARED")
    target_compile_definitions(PulsarionWindowing PUBLIC PULSARION_WINDOWING_BUILD_DLL)
endif()

target_include_directories(PulsarionWindowing PUBLIC src)

# We only require our own libraries
add_dependencies(PulsarionWindowing PulsarionCore)
#add_dependencies(PulsarionWindowing PulsarionMath)
#add_dependencies(PulsarionWindowing PulsarionMedia)
target_link_libraries(PulsarionWindowing PUBLIC Pulsarion::Core)
#target_link_libraries(PulsarionWindowing PUBLIC Pulsarion::Math)
#target_link_libraries(PulsarionWindowing PUBLIC Pulsarion::Media)
