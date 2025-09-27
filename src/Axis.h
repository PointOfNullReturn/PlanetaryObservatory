//
//  Axis.h
//  EarthObservatory
//
//  Created by Kevin Cox on 11/19/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef PLANETARYOBSERVATORY_AXIS_H
#define PLANETARYOBSERVATORY_AXIS_H


#ifndef DEBUG   // For debug output, by default DEBUG is already enabled in XCode
#define DEBUG
#endif

#include "EOGL.h"

// C++ Std Includes
#include <iostream>
#include <cmath>

class Axis
{
public:
    Axis();
    Axis(GLdouble axesLength, GLdouble lineWidth, GLboolean showAxes);
    ~Axis();
    
    // Accessor Methods
    GLboolean GetEnableAxes(void);
    GLdouble GetAxesLineWidth(void);
    GLdouble GetAxesLength(void);
    
    // Mutator Methods
    void SetEnableAxes(GLboolean showAxes);
    void SetAxesLineWidth(GLdouble width);
    void SetAxesLength(GLdouble length);
    
    // Other Methods
    void Render(void);
    
private:

    // Data Members
    GLboolean enableAxes;
    GLdouble axesLineWidth;
    GLdouble axesLength;

};


#endif // PLANETARYOBSERVATORY_AXIS_H
