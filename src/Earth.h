//
//  Earth.h
//  EarthObservatory
//
//  Created by Cox, Kevin on 11/24/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef PLANETARYOBSERVATORY_EARTH_H
#define PLANETARYOBSERVATORY_EARTH_H

#include "EOGL.h"

// C++ Std Includes
#include <string>

// Class Includes
#include "PlanetaryObject.h"

// Sun Globals
// Material Properties


class Earth : public PlanetaryObject
{
public:
    // Constructors
    Earth(std::string objectName);
    ~Earth();
    
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


#endif // PLANETARYOBSERVATORY_EARTH_H
