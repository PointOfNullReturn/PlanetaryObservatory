//
//  EOPlanetaryObject.cpp
//  EarthObservatory
//
//  Created by Kevin Cox on 11/19/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//


#include "EOPlanetaryObject.h"

using namespace ASTRO_MATH_LIB;

// Constructor
EOPlanetaryObject::EOPlanetaryObject(std::string objectName)
{
    m_objectName = objectName;
}

// Destructor
EOPlanetaryObject::~EOPlanetaryObject()
{

}

// Accessor Methods
GLdouble EOPlanetaryObject::GetMass(void)
{
    return m_objectMass;
}

GLdouble EOPlanetaryObject::GetRotationalVelocity(void)
{
    return m_objectRotationalVelocity;
}

GLdouble EOPlanetaryObject::GetObjectRadius(void)
{
    return m_objectRadius;
}

GLdouble EOPlanetaryObject::GetObjectAxisTilt(void)
{
    return m_objectAxisTilt;
}


// Mutator Methods
void EOPlanetaryObject::SetMass(GLdouble massInKG)
{
    m_objectMass = massInKG;
}

void EOPlanetaryObject::SetRotationalVelocity(GLdouble velocityInKMpS)
{
    m_objectRotationalVelocity = velocityInKMpS;
}

void EOPlanetaryObject::SetObjectRadius(GLdouble radiusInKM)
{
    m_objectRadius = radiusInKM;
}

void EOPlanetaryObject::SetObjectAxisTilt(GLdouble tiltInDegrees)
{
    m_objectAxisTilt = tiltInDegrees;
}