//
//  EOMoon.h
//  EarthObservatory
//
//  Created by Kevin Cox on 11/27/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef EarthObservatory_EOMoon_h
#define EarthObservatory_EOMoon_h

#ifndef DEBUG   // For debug output, by default DEBUG is already enabled in XCode
#define DEBUG
#endif

#include "EOGL.h"

// C++ Std Includes
#include <iostream>
#include <cmath>


// Class Includes
#include "EOPlanetaryObject.h"

// Sun Globals
// Material Properties


class EOMoon: public EOPlanetaryObject   
{
public:
    // Constructors
    EOMoon(std::string objectName);
    ~EOMoon(void);
    
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
    
    // Textures
    std::vector<GLuint> m_textures;
    
    // Data Members
    GLfloat m_moonPositionX;
    GLfloat m_moonPositionY;
    GLfloat m_moonPositionZ;
    GLfloat m_moonCurrentOrbitalAngle;
    GLfloat m_moonOrbitalInclination;
    
    GLuint texture;
    GLUquadric *moon;
    GLubyte *wmap;
    
};


#endif
