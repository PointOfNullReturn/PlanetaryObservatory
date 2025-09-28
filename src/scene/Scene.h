//
//  Scene.h
//  EarthObservatory
//
//  Created by Kevin Cox on 11/23/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef PLANETARYOBSERVATORY_SCENE_H
#define PLANETARYOBSERVATORY_SCENE_H

#include "common/EOGL.h"

#include "render/Axis.h"
#include "render/OrbitCamera.h"
#include "scene/Earth.h"
#include "scene/Light.h"
#include "scene/Moon.h"
#include "scene/PlanetaryObject.h"
#include "common/EOGlobalEnums.h"

#include <memory>

class Scene
{
public:
    
    Scene();
    ~Scene();
    
    // Accessor Methods
    RenderModes GetRenderMode(void);
    GLboolean GetShowAxes(void);
    GLboolean GetCurrentlyAnimating(void);
    
    // Mutator Methods
    void SetRenderMode(RenderModes renderMode);
    void SetShowAxes(GLboolean show);
    void SetCurrentlyAnimating(GLboolean animating);
    
    // Other Methods
    void RenderScene(void);
    void UpdateScene(void);
    void InitializeScene(void);
    void HandleKeyboardInput(unsigned char key);
    
private:
    
    // Data Members
    // Scene Objects
    std::unique_ptr<Earth> earth;   // The Earth
    std::unique_ptr<Moon> moon;    // The Moon
    
    // TODO: Implement All Planetary Objects in an array
    //vector<PlanetaryObject*> planetaryObjects;    // List of Planetary Objects to Render
    
    
    // Lighting Objects
    [[maybe_unused]] std::unique_ptr<Light> ambientLight;      // Scene Ambient Lighting
    
    
    // Camera Objects
    [[maybe_unused]] CameraModes currentMode;
    
    // Render Mode
    RenderModes m_renderMode;     // Render the scene in wireframe or solids
    
    // Animating Mode
    GLboolean m_currentlyAnimating;
    
    GLboolean showAxes;     // Whether or not to show the coordinate system axis
    std::unique_ptr<Axis> axes;           // Pointer to the instansiated axes class
    
    
    GLfloat ambientLightColor[4] = {0.50, 0.50, 0.50, 1.0};
    
    GLfloat specularLightColor[4] = {1.0, 1.0, 1.0, 1.0};
    
    
    GLfloat light1Position[4] = {0.0, 0.0, 10.0, 1.0 };
    
    
    std::unique_ptr<OrbitCamera> sceneCamera;
        
};


#endif // PLANETARYOBSERVATORY_SCENE_H
