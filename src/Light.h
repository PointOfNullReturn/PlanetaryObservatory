//
//  Light.h
//  EarthObservatory
//
//  Created by Kevin Cox on 11/25/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef PLANETARYOBSERVATORY_LIGHT_H
#define PLANETARYOBSERVATORY_LIGHT_H

#include "EOGL.h"

// C++ Std Includes
#include <string>

// Other Includes
#include "EOGlobalEnums.h"

class Light
{
public:
    Light(std::string lightName, GLenum lightID);
    ~Light();
    
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






#endif // PLANETARYOBSERVATORY_LIGHT_H
