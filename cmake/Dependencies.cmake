include_guard(GLOBAL)

# Gather dependency include directories and link libraries for the
# PlanetaryObservatory target. Results are returned in the following
# parent-scope variables:
#   PLANETARY_OBSERVATORY_DEP_INCLUDE_DIRS
#   PLANETARY_OBSERVATORY_DEP_LINK_LIBS

set(_planetary_observatory_dep_include_dirs)
set(_planetary_observatory_dep_link_libs)

find_package(OpenGL REQUIRED)

if(TARGET OpenGL::GL)
    list(APPEND _planetary_observatory_dep_link_libs OpenGL::GL)
elseif(TARGET OpenGL::OpenGL)
    list(APPEND _planetary_observatory_dep_link_libs OpenGL::OpenGL)
else()
    message(FATAL_ERROR "Unable to locate a usable OpenGL target")
endif()

if(TARGET OpenGL::GLU)
    list(APPEND _planetary_observatory_dep_link_libs OpenGL::GLU)
else()
    find_library(GLU_LIBRARY GLU)
    if(GLU_LIBRARY)
        list(APPEND _planetary_observatory_dep_link_libs ${GLU_LIBRARY})
    else()
        message(FATAL_ERROR "Unable to locate the GLU library required by legacy rendering")
    endif()
endif()

find_package(glfw3 CONFIG QUIET)

if(glfw3_FOUND)
    if(TARGET glfw)
        list(APPEND _planetary_observatory_dep_link_libs glfw)
    elseif(TARGET glfw::glfw)
        list(APPEND _planetary_observatory_dep_link_libs glfw::glfw)
    else()
        message(FATAL_ERROR "Unable to locate a usable GLFW target from the glfw3 package")
    endif()
else()
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(GLFW3 QUIET glfw3)
    endif()

    if(GLFW3_FOUND)
        list(APPEND _planetary_observatory_dep_include_dirs ${GLFW3_INCLUDE_DIRS})
        list(APPEND _planetary_observatory_dep_link_libs ${GLFW3_LIBRARIES})
    else()
        message(FATAL_ERROR "GLFW not found. Install glfw3 or provide glfw3_DIR to point at a CMake config package.")
    endif()
endif()

set(IMGUI_ROOT "${CMAKE_CURRENT_LIST_DIR}/../third_party/imgui")

if(NOT EXISTS "${IMGUI_ROOT}/imgui.cpp")
    message(FATAL_ERROR "Dear ImGui sources not found under ${IMGUI_ROOT}. Run `git clone https://github.com/ocornut/imgui.git third_party/imgui --branch v1.89.9 --depth 1` from the repository root.")
endif()

set(IMGUI_SOURCES
    ${IMGUI_ROOT}/imgui.cpp
    ${IMGUI_ROOT}/imgui_draw.cpp
    ${IMGUI_ROOT}/imgui_tables.cpp
    ${IMGUI_ROOT}/imgui_widgets.cpp
    ${IMGUI_ROOT}/imgui_demo.cpp
    ${IMGUI_ROOT}/backends/imgui_impl_glfw.cpp
    ${IMGUI_ROOT}/backends/imgui_impl_opengl2.cpp)

add_library(imgui_backend STATIC ${IMGUI_SOURCES})
add_library(imgui::backend ALIAS imgui_backend)
target_include_directories(imgui_backend
    PUBLIC
        ${IMGUI_ROOT}
        ${IMGUI_ROOT}/backends)
target_compile_features(imgui_backend PUBLIC cxx_std_17)

if(TARGET glfw)
    target_link_libraries(imgui_backend PUBLIC glfw)
elseif(TARGET glfw::glfw)
    target_link_libraries(imgui_backend PUBLIC glfw::glfw)
elseif(GLFW3_FOUND)
    target_include_directories(imgui_backend PUBLIC ${GLFW3_INCLUDE_DIRS})
    target_link_libraries(imgui_backend PUBLIC ${GLFW3_LIBRARIES})
endif()

if(TARGET OpenGL::GL)
    target_link_libraries(imgui_backend PUBLIC OpenGL::GL)
elseif(TARGET OpenGL::OpenGL)
    target_link_libraries(imgui_backend PUBLIC OpenGL::OpenGL)
endif()

list(APPEND _planetary_observatory_dep_link_libs imgui::backend)

include(FetchContent)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
)

FetchContent_MakeAvailable(glm)

list(APPEND _planetary_observatory_dep_include_dirs ${glm_SOURCE_DIR})

set(PLANETARY_OBSERVATORY_DEP_INCLUDE_DIRS "${_planetary_observatory_dep_include_dirs}")
set(PLANETARY_OBSERVATORY_DEP_LINK_LIBS "${_planetary_observatory_dep_link_libs}")
