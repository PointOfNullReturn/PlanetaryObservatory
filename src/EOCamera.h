//
//  EOCamera.h
//  EarthObservatory
//
//  Created by Kevin Cox on 11/22/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef EarthObservatory_EOCamera_h
#define EarthObservatory_EOCamera_h

#ifndef DEBUG   // For debug output, by default DEBUG is already enabled in XCode
#define DEBUG
#endif

#include "EOGL.h"

// C++ Std Includes
#include <iostream>
#include <cmath>

// Globals
#include "EOGlobalEnums.h"

static const GLfloat MIN_CAMERA_RADIUS = 2.0f;
static const GLfloat MAX_CAMERA_RADIUS = 38.0f;

class EOCamera 
{
public:
    // Default Camera Constructor
    // 
    EOCamera(void);
    
    // Specific Constructor
    EOCamera(GLdouble position, GLdouble lookAt, GLdouble normal);
    
    ~EOCamera(void);

    
    // Accessor Methods
    
    
    
    // Mutator Methods
    void ChangeLookAt(void);

    void SnapToPreset(int presetIndex);
    void SetRadius(GLfloat radius);
    GLfloat GetRadius(void) const;

    // Other Methods
    void Render(void);
    
    void OrbitLeft(void);
    
    void OrbitRight(void);
    
    void OrbitUp(void);
    
    void OrbitDown(void);
    
    void ZoomIn(void);
    
    void ZoomOut(void);
    
    
    
    
    
private:
    
    CameraModes m_cameraMode;
    
    GLfloat m_cameraRadius;
    
    GLfloat m_cameraOrbitAngle;
    GLfloat m_cameraPitchAngle;
    
    GLfloat m_cameraPositionX;
    GLfloat m_cameraPositionY;
    GLfloat m_cameraPositionZ;
    
    GLfloat m_cameraLookAtPositionX;
    GLfloat m_cameraLookAtPositionY;
    GLfloat m_cameraLookAtPositionZ;
    
    GLfloat m_cameraNormalX;
    GLfloat m_cameraNormalY;
    GLfloat m_cameraNormalZ;
    
    // Methods
    void CalculatePosition(void);
    
    
    
};

#endif
