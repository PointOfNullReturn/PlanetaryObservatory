//
//  Light.cpp
//  EarthObservatory
//
//  Created by Kevin Cox on 11/25/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "scene/Light.h"

Light::Light(std::string lightName, GLenum lightID)
{
    m_lightName = lightName;
    m_lightID = lightID;
    m_lightEnabled = false;
}


Light::~Light()
{
    
}


GLboolean Light::GetLightEnabled(void)
{
    return m_lightEnabled;
}


void Light::SetLightEnabled(GLboolean enable)
{
    m_lightEnabled = enable;
}



void Light::RenderLight()
{
    
}


void Light::InitializeLight(void)
{
    // Initialize the components of the light
}
