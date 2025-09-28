//
//  Earth.cpp
//  EarthObservatory
//
//  Created by Cox, Kevin on 11/24/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "scene/Earth.h"
#include "render/TextureLoader.h"

// Constructors
Earth::Earth(std::string objectName) : PlanetaryObject(objectName)
{
    InitializeObject();
    InitializeMaterials();
    LoadTextures();

    m_rotationAngle = 0.0f;
    
    earth = gluNewQuadric();
    if (earth != nullptr)
    {
        gluQuadricNormals(earth, GLU_SMOOTH);
    }
}


Earth::~Earth()
{
    gluDeleteQuadric(earth);
}


void  Earth::InitializeObject(void)
{
    SetObjectRadius(ASTRO_MATH_LIB::KMtoGU(EARTH_RADIUS_KM));
    SetObjectAxisTilt(23.44);
}


void Earth::UpdateObject(void)
{
    // Advance the planet's axial rotation when animation is enabled.
    constexpr float rotationSpeedDegreesPerFrame = 0.05f;
    m_rotationAngle -= rotationSpeedDegreesPerFrame;
    if (m_rotationAngle <= -360.0f)
    {
        m_rotationAngle += 360.0f;
    }
}


void Earth::RenderObject(RenderModes renderMode)
{
    // Push All Attribute Bits
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    
    // Push Current Matrix
    glPushMatrix();
   
    // Materials
    InitializeMaterials();

    glRotated(90.0 + GetObjectAxisTilt(), 1.0, 0.0, 0.0);
    glRotated(m_rotationAngle, 0.0, 0.0, 1.0);
    
    // If the Scene's View Mode is Wireframe, then the object
    // render's itself in wireframe.
    const bool hasTexture = texture > 0;

    if (renderMode == RENDER_MODE_WIREFRAME)
    {
        gluQuadricTexture(earth, hasTexture ? GL_TRUE : GL_FALSE);
        gluQuadricDrawStyle(earth, GLU_LINE);

        if (hasTexture)
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texture);
        }
        else
        {
            glDisable(GL_TEXTURE_2D);
        }

        gluSphere(earth, GetObjectRadius(), 64, 64);

        if (hasTexture)
        {
            glDisable(GL_TEXTURE_2D);
        }

        gluQuadricDrawStyle(earth, GLU_FILL);
    }
    else
    {
        gluQuadricTexture(earth, hasTexture ? GL_TRUE : GL_FALSE);
        gluQuadricDrawStyle(earth, GLU_FILL);

        if (hasTexture)
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texture);
        }
        else
        {
            glDisable(GL_TEXTURE_2D);
        }

        gluSphere(earth, GetObjectRadius(), 64, 64);

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




void  Earth::InitializeMaterials(void)
{
    GLfloat earth_material_ambient[] = {0.25f, 0.25f, 0.25f, 1.0f};
    GLfloat earth_material_diffused[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat earth_material_specular[] = {0.75f, 0.75f, 0.75f, 1.0f};
    GLfloat earth_material_shininess[] = {185.0f};
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, earth_material_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, earth_material_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, earth_material_diffused);
    glMaterialfv(GL_FRONT, GL_SHININESS, earth_material_shininess);
    
}



GLboolean Earth::LoadTextures(void)
{
    texture = LoadTexture2D("assets/textures/earth_sm.bmp", true, false, true);
    return texture > 0;
}

