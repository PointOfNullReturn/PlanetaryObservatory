//
//  EOLight.h
//  EarthObservatory
//
//  Created by Kevin Cox on 11/25/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef EarthObservatory_EOLight_h
#define EarthObservatory_EOLight_h

#ifndef DEBUG   // For debug output, by default DEBUG is already enabled in XCode
#define DEBUG
#endif

#include "EOGL.h"

// C++ Std Includes
#include <iostream>
#include <cmath>

// Other Includes
#include "EOGlobalEnums.h"

class EOLight
{
public:
    EOLight(std::string lightName, GLenum lightID);
    ~EOLight();
    
    // Accessor Methods
    GLboolean GetLightEnabled(void);
    
    // Mutator Methods
    void SetLightEnabled(GLboolean enable);
    void SetLightPosition(void);
    
    // Other
    void RenderLight(void);
    
private:
    std::string m_lightName;
    GLenum m_lightID;
    GLboolean m_lightEnabled;
    
    
    void InitializeLight(void);

    
};






#endif
