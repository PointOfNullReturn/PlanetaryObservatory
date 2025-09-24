//
//  EOCamera.cpp
//  EarthObservatory
//
//  Created by Kevin Cox on 11/22/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "EOCamera.h"
#include <algorithm>

EOCamera::EOCamera(void)
{
    // Orbit Mode
    m_cameraMode = CAMERA_ORBIT;

    // Camera Starting Orbital Radius (stay outside the Earth by default)
    m_cameraRadius = 5.0f;

    m_cameraOrbitAngle = 270.0f;
    m_cameraPitchAngle = 0.0f;

    m_cameraLookAtPositionX = 0.0f;
    m_cameraLookAtPositionY = 0.0f;
    m_cameraLookAtPositionZ = 0.0f;

    m_cameraNormalX = 0.0f;
    m_cameraNormalY = 1.0f;
    m_cameraNormalZ = 0.0f;

    CalculatePosition();
}


EOCamera::EOCamera(GLdouble position, GLdouble lookAt, GLdouble normal)
{


}


void EOCamera::Render(void)
{
    gluLookAt(m_cameraPositionX, m_cameraPositionY, m_cameraPositionZ,
              m_cameraLookAtPositionX, m_cameraLookAtPositionY, m_cameraLookAtPositionZ,
              m_cameraNormalX, m_cameraNormalY, m_cameraNormalZ);

}

void EOCamera::OrbitLeft(void)
{

}

void EOCamera::OrbitRight(void)
{

}

void EOCamera::OrbitUp(void)
{

}

void EOCamera::OrbitDown(void)
{

}

void EOCamera::ZoomIn(void)
{
    if (!(m_cameraRadius <= MIN_CAMERA_RADIUS))
    {
        m_cameraRadius -= 0.025;

        CalculatePosition();
    }

}

void EOCamera::ZoomOut(void)
{
    if (!(m_cameraRadius >= MAX_CAMERA_RADIUS))
    {
        m_cameraRadius += 0.025;

        CalculatePosition();
    }
}

void EOCamera::ChangeLookAt(void)
{

}


void EOCamera::CalculatePosition(void)
{
    const GLdouble degToRad = 3.14159265358979323846 / 180.0;
    const GLdouble yaw = m_cameraOrbitAngle * degToRad;
    const GLdouble pitch = m_cameraPitchAngle * degToRad;

    const GLdouble cosPitch = std::cos(pitch);
    const GLdouble sinPitch = std::sin(pitch);
    const GLdouble cosYaw = std::cos(yaw);
    const GLdouble sinYaw = std::sin(yaw);

    m_cameraPositionX = static_cast<GLfloat>(m_cameraRadius * cosPitch * cosYaw);
    m_cameraPositionY = static_cast<GLfloat>(m_cameraRadius * sinPitch);
    m_cameraPositionZ = static_cast<GLfloat>(m_cameraRadius * cosPitch * sinYaw);
}

void EOCamera::SetRadius(GLfloat radius)
{
    m_cameraRadius = std::clamp(radius, MIN_CAMERA_RADIUS, MAX_CAMERA_RADIUS);
    CalculatePosition();
}

void EOCamera::SnapToPreset(int presetIndex)
{
    switch (presetIndex)
    {
        case 0: // +X
            m_cameraOrbitAngle = 0.0f;
            m_cameraPitchAngle = 0.0f;
            m_cameraNormalX = 0.0f;
            m_cameraNormalY = 1.0f;
            m_cameraNormalZ = 0.0f;
            break;
        case 1: // -X
            m_cameraOrbitAngle = 180.0f;
            m_cameraPitchAngle = 0.0f;
            m_cameraNormalX = 0.0f;
            m_cameraNormalY = 1.0f;
            m_cameraNormalZ = 0.0f;
            break;
        case 2: // +Y
            m_cameraOrbitAngle = 0.0f;
            m_cameraPitchAngle = 90.0f;
            m_cameraNormalX = 0.0f;
            m_cameraNormalY = 0.0f;
            m_cameraNormalZ = -1.0f;
            break;
        case 3: // -Y
            m_cameraOrbitAngle = 0.0f;
            m_cameraPitchAngle = -90.0f;
            m_cameraNormalX = 0.0f;
            m_cameraNormalY = 0.0f;
            m_cameraNormalZ = 1.0f;
            break;
        case 4: // +Z
            m_cameraOrbitAngle = 90.0f;
            m_cameraPitchAngle = 0.0f;
            m_cameraNormalX = 0.0f;
            m_cameraNormalY = 1.0f;
            m_cameraNormalZ = 0.0f;
            break;
        case 5: // -Z
        default:
            m_cameraOrbitAngle = -90.0f;
            m_cameraPitchAngle = 0.0f;
            m_cameraNormalX = 0.0f;
            m_cameraNormalY = 1.0f;
            m_cameraNormalZ = 0.0f;
            break;
    }

    CalculatePosition();
}

GLfloat EOCamera::GetRadius(void) const
{
    return m_cameraRadius;
}
