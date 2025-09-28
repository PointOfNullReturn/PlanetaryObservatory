/*
********************************************************************************
 
 Filename: astromathlib.cpp
 Type: Implementation File
 
 
 Purpose: To implement a namespace for a math library to handle calculations 
 dealing with astrophysics to OpenGL graphics.
 
*******************************************************************************/

#include "math/astromathlib.h"

#include "utils/Log.h"

// Convert Kilometers to Astronomical Units (1 AU = 149,597,870.7 km)
GLdouble ASTRO_MATH_LIB::KMtoGU(GLdouble kilometers)
{
    return kilometers * (1.0/KM_TO_GU_SCALE);
}
