//
//  EOMoon.cpp
//  EarthObservatory
//
//  Created by Kevin Cox on 11/27/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "EOMoon.h"
#include "TextureLoader.h"

// Constructors
EOMoon::EOMoon(std::string objectName) : EOPlanetaryObject(objectName)
{

    
    InitializeObject();
    InitializeMaterials();
    LoadTextures();
    
    moon = gluNewQuadric();
    if (moon != nullptr)
    {
        gluQuadricNormals(moon, GLU_SMOOTH);
    }
}


EOMoon::~EOMoon(void)
{
    
}


void  EOMoon::InitializeObject(void)
{
    SetObjectRadius(0.50);
    SetObjectAxisTilt(0.0);
    
    m_moonCurrentOrbitalAngle = 180.0;
    
    // Initialize the position
    GLfloat orbitRadius = (ASTRO_MATH_LIB::KMtoGU(384000 + EARTH_RADIUS_KM));
    
    std::cout << "Moon Orbit Angle: " << m_moonCurrentOrbitalAngle << std::endl;
    std::cout << "Moon Orbit Radius: " << orbitRadius << std::endl;
    
    m_moonPositionX = orbitRadius * cos(m_moonCurrentOrbitalAngle);
    m_moonPositionZ = orbitRadius * sin(m_moonCurrentOrbitalAngle);
    m_moonPositionY = 0.0;
}


void EOMoon::UpdateObject(void)
{
    // For Animation Purposes
    
    // Moon Orbit
    // General Equation
    // xPos = r * cos(angle), zPos = r * sin(angle)
    // yPos is the Orbital Inclination
    
    // Step the orbital Angle
    if (m_moonCurrentOrbitalAngle >= 360.0)
        m_moonCurrentOrbitalAngle = 0.0;
    else
        m_moonCurrentOrbitalAngle += 0.008;
    
    // This gives us the orbit radius from 0,0,0
    GLfloat orbitRadius = (ASTRO_MATH_LIB::KMtoGU(384000 + EARTH_RADIUS_KM));
    
    std::cout << "Moon Orbit Angle: " << m_moonCurrentOrbitalAngle << std::endl;
    std::cout << "Moon Orbit Radius: " << orbitRadius << std::endl;
    
    m_moonPositionX = orbitRadius * cos(m_moonCurrentOrbitalAngle);
    m_moonPositionZ = orbitRadius * sin(m_moonCurrentOrbitalAngle);
    m_moonPositionY = 0.0;
}


void EOMoon::RenderObject(RenderModes renderMode)
{
    
    // Push All Attribute Bits
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    
    // Push Current Matrix
    glPushMatrix();
    
    // Materials
    InitializeMaterials();
    
    glTranslatef(m_moonPositionX, m_moonPositionY, m_moonPositionZ);
    glRotated(90.0 + GetObjectAxisTilt(), 1.0, 0.0, 0.0);
    
    std::cout << "Moon xPos: " << m_moonPositionX << " zPos: " << m_moonPositionZ << std::endl;
    
    // If the Scene's View Mode is Wireframe, then the object
    // render's itself in wireframe.
    const bool hasTexture = texture > 0;

    if (renderMode == RENDER_MODE_WIREFRAME)
    {
        gluQuadricTexture(moon, hasTexture ? GL_TRUE : GL_FALSE);
        gluQuadricDrawStyle(moon, GLU_LINE);

        if (hasTexture)
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texture);
        }
        else
        {
            glDisable(GL_TEXTURE_2D);
        }

        gluSphere(moon, GetObjectRadius(), 64, 64);

        if (hasTexture)
        {
            glDisable(GL_TEXTURE_2D);
        }

        gluQuadricDrawStyle(moon, GLU_FILL);
    }
    else
    {
        gluQuadricTexture(moon, hasTexture ? GL_TRUE : GL_FALSE);
        gluQuadricDrawStyle(moon, GLU_FILL);

        if (hasTexture)
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texture);
        }
        else
        {
            glDisable(GL_TEXTURE_2D);
        }

        gluSphere(moon, GetObjectRadius(), 64, 64);

        if (hasTexture)
        {
            glDisable(GL_TEXTURE_2D);
        }
    }
    
    
    // Pop Current Matrix
    glPopMatrix();
    
    // Pop All Attribute Bits
    glPopAttrib();
    
}




void  EOMoon::InitializeMaterials(void)
{
    GLfloat moon_material_ambient[] = {0.50f, 0.50f, 0.50f, 1.0f};
    GLfloat moon_material_diffused[] = {0.75f, 0.75f, 0.75f, 1.0f};
    GLfloat moon_material_specular[] = {0.75f, 0.75f, 0.75f, 1.0f};
    GLfloat moon_material_shininess[] = {85.0f};
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, moon_material_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, moon_material_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, moon_material_diffused);
    glMaterialfv(GL_FRONT, GL_SHININESS, moon_material_shininess);
    
}



GLboolean EOMoon::LoadTextures(void)
{
    texture = LoadTexture2D("assets/textures/moon_sm.bmp", true, false);
    return texture > 0;
}