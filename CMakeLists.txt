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

set(PLANETARY_OBSERVATORY_SOURCES
    src/Axis.cpp
    src/OrbitCamera.cpp
    src/Earth.cpp
    src/Light.cpp
    src/Moon.cpp
    src/PlanetaryObject.cpp
    src/Scene.cpp
    src/core/Application.cpp
    src/layers/SceneLayer.cpp
    src/TextureLoader.cpp
    src/astromathlib.cpp
    src/camera.cpp
    src/main.cpp
    src/mathlib.cpp
)

add_executable(${PROJECT_NAME} ${PLANETARY_OBSERVATORY_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE src third_party/stb)

find_package(OpenGL REQUIRED)

if(TARGET OpenGL::GL)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)
elseif(TARGET OpenGL::OpenGL)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::OpenGL)
else()
    message(FATAL_ERROR "Unable to locate a usable OpenGL target")
endif()

if(TARGET OpenGL::GLU)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GLU)
else()
    find_library(GLU_LIBRARY GLU)
    if(GLU_LIBRARY)
        target_link_libraries(${PROJECT_NAME} PRIVATE ${GLU_LIBRARY})
    else()
        message(FATAL_ERROR "Unable to locate the GLU library required by legacy rendering")
    endif()
endif()

find_package(glfw3 CONFIG QUIET)

if(glfw3_FOUND)
    if(TARGET glfw)
        target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
    elseif(TARGET glfw::glfw)
        target_link_libraries(${PROJECT_NAME} PRIVATE glfw::glfw)
    else()
        message(FATAL_ERROR "Unable to locate a usable GLFW target from the glfw3 package")
    endif()
else()
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(GLFW3 QUIET glfw3)
    endif()

    if(GLFW3_FOUND)
        target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW3_INCLUDE_DIRS})
        target_link_libraries(${PROJECT_NAME} PRIVATE ${GLFW3_LIBRARIES})
    else()
        message(FATAL_ERROR "GLFW not found. Install glfw3 or provide glfw3_DIR to point at a CMake config package.")
    endif()
endif()

if(APPLE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE GL_SILENCE_DEPRECATION)
endif()
