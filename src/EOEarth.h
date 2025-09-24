//
//  EOEarth.h
//  EarthObservatory
//
//  Created by Cox, Kevin on 11/24/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef EarthObservatory_EOEarth_h
#define EarthObservatory_EOEarth_h

#ifndef DEBUG   // For debug output, by default DEBUG is already enabled in XCode
#define DEBUG
#endif

#include "EOGL.h"

// C++ Std Includes
#include <string>
#include <vector>

// Class Includes
#include "EOPlanetaryObject.h"

// Sun Globals
// Material Properties


class EOEarth: public EOPlanetaryObject   
{
public:
    // Constructors
    EOEarth(std::string objectName);
    ~EOEarth(void);
    
    // Accessors
    
    
    // Mutators
    
    
    // Other
    void RenderObject(RenderModes renderMode);
    void UpdateObject(void);
    
    
private:
    
    // Other Methods
    GLboolean LoadTextures(void);
    void InitializeObject(void);
    void InitializeMaterials(void);    
    
    GLuint texture;
    GLUquadric* earth;
    GLfloat m_rotationAngle;
    
};


#endif
