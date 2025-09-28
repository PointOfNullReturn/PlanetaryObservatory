//
//  Moon.h
//  EarthObservatory
//
//  Created by Kevin Cox on 11/27/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef PLANETARYOBSERVATORY_MOON_H
#define PLANETARYOBSERVATORY_MOON_H

#include "common/EOGL.h"

// C++ Std Includes
#include <string>
#include <vector>


// Class Includes
#include "scene/PlanetaryObject.h"

// Sun Globals
// Material Properties


class Moon : public PlanetaryObject
{
public:
    // Constructors
    Moon(std::string objectName);
    ~Moon();
    
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
    GLuint texture;
    GLUquadric *moon;
    
};


#endif // PLANETARYOBSERVATORY_MOON_H
