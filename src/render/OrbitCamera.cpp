//
//  OrbitCamera.cpp
//  EarthObservatory
//
//  Created by Kevin Cox on 11/22/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "render/OrbitCamera.h"

#include <algorithm>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

OrbitCamera::OrbitCamera()
{
    m_cameraMode = CAMERA_ORBIT;
    m_cameraRadius = 5.0f;
    m_cameraOrbitAngle = 270.0f;
    m_cameraPitchAngle = 0.0f;
    m_cameraLookAtPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    m_cameraNormal = glm::vec3(0.0f, 1.0f, 0.0f);

    CalculatePosition();
}

glm::mat4 OrbitCamera::GetViewMatrix() const {
    return glm::lookAt(m_cameraPosition, m_cameraLookAtPosition, m_cameraNormal);
}

void OrbitCamera::OrbitLeft(void)
{

}

void OrbitCamera::OrbitRight(void)
{

}

void OrbitCamera::OrbitUp(void)
{

}

void OrbitCamera::OrbitDown(void)
{

}

void OrbitCamera::ZoomIn(void)
{
    if (!(m_cameraRadius <= MIN_CAMERA_RADIUS))
    {
        m_cameraRadius -= 0.025;

        CalculatePosition();
    }

}

void OrbitCamera::ZoomOut(void)
{
    if (!(m_cameraRadius >= MAX_CAMERA_RADIUS))
    {
        m_cameraRadius += 0.025;

        CalculatePosition();
    }
}

void OrbitCamera::ChangeLookAt(void)
{

}


void OrbitCamera::CalculatePosition(void)
{
    const double yaw = glm::radians(m_cameraOrbitAngle);
    const double pitch = glm::radians(m_cameraPitchAngle);

    const double cosPitch = std::cos(pitch);
    const double sinPitch = std::sin(pitch);
    const double cosYaw = std::cos(yaw);
    const double sinYaw = std::sin(yaw);

    m_cameraPosition.x = static_cast<float>(m_cameraRadius * cosPitch * cosYaw);
    m_cameraPosition.y = static_cast<float>(m_cameraRadius * sinPitch);
    m_cameraPosition.z = static_cast<float>(m_cameraRadius * cosPitch * sinYaw);
}

void OrbitCamera::SetRadius(GLfloat radius)
{
    m_cameraRadius = std::clamp(radius, MIN_CAMERA_RADIUS, MAX_CAMERA_RADIUS);
    CalculatePosition();
}

void OrbitCamera::SnapToPreset(int presetIndex)
{
    switch (presetIndex)
    {
        case 0: // +X
            m_cameraOrbitAngle = 0.0f;
            m_cameraPitchAngle = 0.0f;
            m_cameraNormal = glm::vec3(0.0f, 1.0f, 0.0f);
            break;
        case 1: // -X
            m_cameraOrbitAngle = 180.0f;
            m_cameraPitchAngle = 0.0f;
            m_cameraNormal = glm::vec3(0.0f, 1.0f, 0.0f);
            break;
        case 2: // +Y
            m_cameraOrbitAngle = 0.0f;
            m_cameraPitchAngle = 90.0f;
            m_cameraNormal = glm::vec3(0.0f, 0.0f, -1.0f);
            break;
        case 3: // -Y
            m_cameraOrbitAngle = 0.0f;
            m_cameraPitchAngle = -90.0f;
            m_cameraNormal = glm::vec3(0.0f, 0.0f, 1.0f);
            break;
        case 4: // +Z
            m_cameraOrbitAngle = 90.0f;
            m_cameraPitchAngle = 0.0f;
            m_cameraNormal = glm::vec3(0.0f, 1.0f, 0.0f);
            break;
        case 5: // -Z
        default:
            m_cameraOrbitAngle = -90.0f;
            m_cameraPitchAngle = 0.0f;
            m_cameraNormal = glm::vec3(0.0f, 1.0f, 0.0f);
            break;
    }

    CalculatePosition();
}

GLfloat OrbitCamera::GetRadius(void) const
{
    return m_cameraRadius;
}
