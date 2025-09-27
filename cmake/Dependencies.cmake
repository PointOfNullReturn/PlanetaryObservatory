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

set(PLANETARY_OBSERVATORY_DEP_INCLUDE_DIRS "${_planetary_observatory_dep_include_dirs}")
set(PLANETARY_OBSERVATORY_DEP_LINK_LIBS "${_planetary_observatory_dep_link_libs}")
