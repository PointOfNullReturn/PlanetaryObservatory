//
//  PlanetaryObject.cpp
//  EarthObservatory
//
//  Created by Kevin Cox on 11/19/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//


#include "PlanetaryObject.h"

using namespace ASTRO_MATH_LIB;

// Constructor
PlanetaryObject::PlanetaryObject(std::string objectName)
{
    m_objectName = objectName;
}

// Accessor Methods
GLdouble PlanetaryObject::GetMass(void)
{
    return m_objectMass;
}

GLdouble PlanetaryObject::GetRotationalVelocity(void)
{
    return m_objectRotationalVelocity;
}

GLdouble PlanetaryObject::GetObjectRadius(void)
{
    return m_objectRadius;
}

GLdouble PlanetaryObject::GetObjectAxisTilt(void)
{
    return m_objectAxisTilt;
}


// Mutator Methods
void PlanetaryObject::SetMass(GLdouble massInKG)
{
    m_objectMass = massInKG;
}

void PlanetaryObject::SetRotationalVelocity(GLdouble velocityInKMpS)
{
    m_objectRotationalVelocity = velocityInKMpS;
}

void PlanetaryObject::SetObjectRadius(GLdouble radiusInKM)
{
    m_objectRadius = radiusInKM;
}

void PlanetaryObject::SetObjectAxisTilt(GLdouble tiltInDegrees)
{
    m_objectAxisTilt = tiltInDegrees;
}
