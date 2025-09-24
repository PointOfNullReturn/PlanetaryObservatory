//
//  EOPlanetaryObject.h
//  EarthObservatory
//
//  Created by Kevin Cox on 11/19/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef EarthObservatory_EOObject_h
#define EarthObservatory_EOObject_h

#ifndef DEBUG   // For debug output, by default DEBUG is already enabled in XCode
#define DEBUG
#endif

#include "EOGL.h"

// C++ Std Includes
#include <iostream>
#include <vector>
#include <cmath>

// Constants
#include "EOGlobalEnums.h"
#include "EOPlanetaryConstants.h"

// AstroMath Library
#include "astromathlib.h"

class EOPlanetaryObject
{
public:
    
    // Constructor
    explicit EOPlanetaryObject(std::string objectName);
    
    // Destructor
    virtual ~EOPlanetaryObject() = default;
    
    // Accessor Methods
    GLdouble GetMass();
    GLdouble GetRotationalVelocity();
    GLdouble GetObjectRadius();
    GLdouble GetObjectAxisTilt();
    
    
    // Mutator Methods
    void SetMass(GLdouble massInKG);
    void SetRotationalVelocity(GLdouble velocityInKMpSec);
    void SetObjectRadius(GLdouble radiusInKM);
    void SetObjectAxisTilt(GLdouble tiltInDegrees);
    
    // Other Methods
    virtual void RenderObject(RenderModes) = 0;
    virtual void UpdateObject(void) = 0;
    

    
private:
    
    // Other Methods
    virtual void InitializeObject() = 0;
    virtual void InitializeMaterials() = 0;
    virtual GLboolean LoadTextures() = 0;
    
    // Object Name
    std::string m_objectName;
    
    // Data about the "real" object this graphical object represents
    GLdouble m_objectRadius;        // The Object's Equatorial Radius
    GLdouble m_objectMass;                // The Object's Mass
    GLdouble m_objectRotationalVelocity;  // The Object's Rotational Velocity
    GLdouble m_objectAxisTilt;            // The Object's Axis Tilt
    
    positionalVector m_objectPosition;    // The Object's position in the scene
    

    
};


#endif
