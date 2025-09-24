//
//  EOLight.cpp
//  EarthObservatory
//
//  Created by Kevin Cox on 11/25/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "EOLight.h"

EOLight::EOLight(std::string lightName, GLenum lightID)
{
    m_lightName = lightName;
    m_lightID = lightID;
    m_lightEnabled = false;
}


EOLight::~EOLight()
{
    
}


GLboolean EOLight::GetLightEnabled(void)
{
    return m_lightEnabled;
}


void EOLight::SetLightEnabled(GLboolean enable)
{
    m_lightEnabled = enable;
}



void EOLight::RenderLight()
{
    
}


void EOLight::InitializeLight(void)
{
    // Initialize the components of the light
}