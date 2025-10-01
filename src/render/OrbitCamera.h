//
//  OrbitCamera.h
//  EarthObservatory
//
//  Created by Kevin Cox on 11/22/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef PLANETARYOBSERVATORY_ORBITCAMERA_H
#define PLANETARYOBSERVATORY_ORBITCAMERA_H

#include "common/EOGL.h"
#include "common/EOGlobalEnums.h"
#include <glm/glm.hpp>

static const GLfloat MIN_CAMERA_RADIUS = 2.0f;
static const GLfloat MAX_CAMERA_RADIUS = 38.0f;

class OrbitCamera
{
public:
    OrbitCamera();
    ~OrbitCamera() = default;

    void ChangeLookAt(void);

    void SnapToPreset(int presetIndex);
    void SetRadius(GLfloat radius);
    GLfloat GetRadius(void) const;

    void OrbitLeft(void);
    void OrbitRight(void);
    void OrbitUp(void);
    void OrbitDown(void);
    
    void ZoomIn(void);
    void ZoomOut(void);

    const glm::vec3 &GetPosition() const { return m_cameraPosition; }
    const glm::vec3 &GetLookAt() const { return m_cameraLookAtPosition; }
    const glm::vec3 &GetUp() const { return m_cameraNormal; }
    glm::mat4 GetViewMatrix() const;

private:
    CameraModes m_cameraMode;
    
    GLfloat m_cameraRadius;
    
    GLfloat m_cameraOrbitAngle;
    GLfloat m_cameraPitchAngle;
    
    glm::vec3 m_cameraPosition;
    glm::vec3 m_cameraLookAtPosition;
    glm::vec3 m_cameraNormal;
    
    void CalculatePosition(void);
};
#endif // PLANETARYOBSERVATORY_ORBITCAMERA_H
