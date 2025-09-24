//
//  EOAxis.h
//  EarthObservatory
//
//  Created by Kevin Cox on 11/19/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef EarthObservatory_EOAxis_h
#define EarthObservatory_EOAxis_h


#ifndef DEBUG   // For debug output, by default DEBUG is already enabled in XCode
#define DEBUG
#endif

#include "EOGL.h"

// C++ Std Includes
#include <iostream>
#include <cmath>

class EOAxis
{
public:
    EOAxis(void);
    EOAxis(GLdouble axesLength, GLdouble lineWidth, GLboolean showAxes); 
    ~EOAxis(void);
    
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


#endif
