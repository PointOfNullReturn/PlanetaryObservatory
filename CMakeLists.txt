cmake_minimum_required(VERSION 3.16)
project(PlanetaryObservatory LANGUAGES CXX)
if(APPLE)
    if(EXISTS "/opt/homebrew/lib/cmake")
        list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")
    endif()
    if(EXISTS "/usr/local/lib/cmake")
        list(APPEND CMAKE_PREFIX_PATH "/usr/local")
    endif()
endif()


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/Dependencies.cmake)
include(cmake/Warnings.cmake)

set(PLANETARY_OBSERVATORY_SOURCES
    src/Axis.cpp
    src/OrbitCamera.cpp
    src/Earth.cpp
    src/Light.cpp
    src/Moon.cpp
    src/PlanetaryObject.cpp
    src/Scene.cpp
    src/utils/Log.cpp
    src/core/Application.cpp
    src/layers/SceneLayer.cpp
    src/TextureLoader.cpp
    src/astromathlib.cpp
    src/camera.cpp
    src/main.cpp
    src/mathlib.cpp
)

add_executable(${PROJECT_NAME} ${PLANETARY_OBSERVATORY_SOURCES})

po_enable_default_warnings(${PROJECT_NAME})

set(PLANETARY_OBSERVATORY_PRIVATE_INCLUDE_DIRS
    src
    third_party/stb
)

if(PLANETARY_OBSERVATORY_DEP_INCLUDE_DIRS)
    list(APPEND PLANETARY_OBSERVATORY_PRIVATE_INCLUDE_DIRS
        ${PLANETARY_OBSERVATORY_DEP_INCLUDE_DIRS})
endif()

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PLANETARY_OBSERVATORY_PRIVATE_INCLUDE_DIRS}
)

if(PLANETARY_OBSERVATORY_DEP_LINK_LIBS)
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            ${PLANETARY_OBSERVATORY_DEP_LINK_LIBS})
endif()

if(APPLE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE GL_SILENCE_DEPRECATION)
endif()
