//
//  Scene.cpp
//  EarthObservatory
//
//  Created by Kevin Cox on 11/23/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "scene/Scene.h"

#include "utils/Log.h"

#include <memory>
#include <string>



Scene::Scene()
{
    
    // Instansiate the Earth Planetary Object
    earth = std::make_unique<Earth>("Earth");
    
    // Instansiate the Moon Planetary Object
    moon = std::make_unique<Moon>("Moon");
    
    sceneCamera = std::make_unique<OrbitCamera>();
    
    //TODO: Finish Lighting Object
    //ambientLight = std::make_unique<Light>("LIGHT0", GL_LIGHT0);
    InitializeScene();
    
    // This all needs to go in a scene init function.
    axes = std::make_unique<Axis>(10.0, 2.0, false);
    
    if (axes->GetEnableAxes())
        showAxes = true;
    else
        showAxes = false;
    
    m_currentlyAnimating = false;

}


Scene::~Scene()
{
    
}


void Scene::InitializeScene(void)
{
    // Initialize Background Color
    // Set Background Color to Black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    // Enable GL Features
    // Enable Line Smoothing
    glEnable(GL_LINE_SMOOTH_HINT);
    
    // Enable Depth Test
    glEnable(GL_DEPTH_TEST);
    
    // Enable Cull Face
    glEnable(GL_CULL_FACE);
    
    // Enable Shading
    glShadeModel(GL_SMOOTH);
    
    // Enable Normalization
    glEnable(GL_NORMALIZE);
    
    // Enable Lighting
    glEnable(GL_LIGHTING);
    
    //glEnable(GL_COLOR_MATERIAL);
    
    //glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    
    // Initialize Lighting
    // LIGHT_0 - Ambient Light
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLightColor);
    
    // LIGHT_1 - Specular Light
    glLightfv(GL_LIGHT1, GL_SPECULAR, specularLightColor);
    
    
}

// Accessor Methods
RenderModes Scene::GetRenderMode(void)
{
    return m_renderMode;
}

GLboolean Scene::GetShowAxes(void)
{
    return showAxes;
}

GLboolean Scene::GetCurrentlyAnimating(void)
{
    return m_currentlyAnimating;
}

// Mutator Methods
void Scene::SetRenderMode(RenderModes renderMode)
{
    m_renderMode = renderMode;
}

void Scene::SetShowAxes(GLboolean show)
{
    showAxes = show;
    axes->SetEnableAxes(showAxes);
}

void Scene::SetCurrentlyAnimating(GLboolean animating)
{
    m_currentlyAnimating = animating;
}

// Other Methods
void Scene::RenderScene(void)
{
    // Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    
    // Third Party Camera
   // sceneCamera.Render();
    /*
    gluLookAt(3.0, 0.0, ASTRO_MATH_LIB::KMtoGU(10000.0 + EARTH_RADIUS_KM),
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);*/
    
    sceneCamera->Render();
    
    glEnable(GL_LIGHT0);
    
    glPushMatrix();
    glLightfv(GL_LIGHT1, GL_POSITION, light1Position);
    glEnable(GL_LIGHT1);
    glPopMatrix();
    
    //glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    
    if (m_renderMode == RENDER_MODE_NORMAL)
    {
        earth->RenderObject(RENDER_MODE_NORMAL);
        moon->RenderObject(RENDER_MODE_NORMAL);
    }
    else
    {
        earth->RenderObject(RENDER_MODE_WIREFRAME);
        moon->RenderObject(RENDER_MODE_WIREFRAME);
    }
    glPopMatrix();
    
    if (axes->GetEnableAxes())
    {
        axes->SetAxesLength(sceneCamera->GetRadius());
        Log::debug("Rendering axes");
        axes->Render();
        
    }
    
    // Flush drawing commands handled by caller.
}


void Scene::UpdateScene(void)
{
    // If currently animating, update all objects
    moon->UpdateObject();
    earth->UpdateObject();
}

void Scene::HandleKeyboardInput(unsigned char key)
{
    switch (key) {
        
        // Scene Edit Helper Keys
        // Show Wireframe
        case 'w':
            if (GetRenderMode() == RENDER_MODE_NORMAL)
                SetRenderMode(RENDER_MODE_WIREFRAME);
            else
                SetRenderMode(RENDER_MODE_NORMAL);
            break;
        
        // Show Axes
        case 'x':
            if (GetShowAxes())
                SetShowAxes(false);
            else
                SetShowAxes(true);
            break;
            

        // Preset views along world axes
        case '1': // +X
            sceneCamera->SnapToPreset(0);
            break;

        case '2': // -X
            sceneCamera->SnapToPreset(1);
            break;

        case '3': // +Y
            sceneCamera->SnapToPreset(2);
            break;

        case '4': // -Y
            sceneCamera->SnapToPreset(3);
            break;

        case '5': // +Z
            sceneCamera->SnapToPreset(4);
            break;

        case '6': // -Z
            sceneCamera->SnapToPreset(5);
            break;

        // Move Closer
        case '+':
            sceneCamera->ZoomIn();
            break;

        // Move Back
        case '-':
            sceneCamera->ZoomOut();
            break;
        default:
            if (Log::kDebugLoggingEnabled)
            {
                Log::debug(std::string("Unhandled Keyboard Key: ") +
                           static_cast<char>(key));
            }
            break;
    }
}
