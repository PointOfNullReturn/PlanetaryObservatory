/*
********************************************************************************

 Filename: astromathlib.h
 Type: Header File
 
 
 Purpose: To define and create a math library to handle calculations dealing
 with astrophysics to OpenGL graphics.
 
 
*******************************************************************************/
#ifndef EarthObservatory_astromathlib_h
#define EarthObservatory_astromathlib_h

#include "common/EOGL.h"

#include <iostream>
#include <cmath>

namespace ASTRO_MATH_LIB 
{
    /*
    --------------------------------------------------------------------------------
     Constants
    ------------------------------------------------------------------------------*/
    // The Scale used to map KM to Graphical Units
    // 10,000KM = 1 GU
    static const GLdouble KM_TO_GU_SCALE = 10000.0;
    
    
    
    /*
    --------------------------------------------------------------------------------
     Function Prototypes
    ------------------------------------------------------------------------------*/     
    // Convert Kilometers to Graphical Units
    GLdouble KMtoGU(GLdouble kilometers);
    
    void test(void);
    
    
}
#endif
