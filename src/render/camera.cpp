//-----------------------------------------------------------------------------
// Copyright (c) 2007-2008 dhpoware. All Rights Reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#include <algorithm>
#include "render/camera.h"

const float Camera::DEFAULT_ROTATION_SPEED = 0.3f;
const float Camera::DEFAULT_FOVX = 90.0f;
const float Camera::DEFAULT_ZNEAR = 0.1f;
const float Camera::DEFAULT_ZFAR = 1000.0f;

const float Camera::DEFAULT_ORBIT_MIN_ZOOM = DEFAULT_ZNEAR + 1.0f;
const float Camera::DEFAULT_ORBIT_MAX_ZOOM = DEFAULT_ZFAR * 0.5f;

const float Camera::DEFAULT_ORBIT_OFFSET_DISTANCE = DEFAULT_ORBIT_MIN_ZOOM +
    (DEFAULT_ORBIT_MAX_ZOOM - DEFAULT_ORBIT_MIN_ZOOM) * 0.25f;

const glm::vec3 Camera::WORLD_XAXIS(1.0f, 0.0f, 0.0f);
const glm::vec3 Camera::WORLD_YAXIS(0.0f, 1.0f, 0.0f);
const glm::vec3 Camera::WORLD_ZAXIS(0.0f, 0.0f, 1.0f);

Camera::Camera()
{
    m_behavior = CAMERA_BEHAVIOR_FLIGHT;
    m_preferTargetYAxisOrbiting = true;
    
    m_accumPitchDegrees = 0.0f;
    m_savedAccumPitchDegrees = 0.0f;
    
    m_rotationSpeed = DEFAULT_ROTATION_SPEED;
    m_fovx = DEFAULT_FOVX;
    m_aspectRatio = 0.0f;
    m_znear = DEFAULT_ZNEAR;
    m_zfar = DEFAULT_ZFAR;   
    
    m_orbitMinZoom = DEFAULT_ORBIT_MIN_ZOOM;
    m_orbitMaxZoom = DEFAULT_ORBIT_MAX_ZOOM;
    m_orbitOffsetDistance = DEFAULT_ORBIT_OFFSET_DISTANCE;
        
    m_eye = glm::vec3(0.0f, 0.0f, 0.0f);
    m_savedEye = glm::vec3(0.0f, 0.0f, 0.0f);
    m_target = glm::vec3(0.0f, 0.0f, 0.0f);
    m_xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
    m_yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    m_targetYAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    m_zAxis = glm::vec3(0.0f, 0.0f, 1.0f);
    m_viewDir = glm::vec3(0.0f, 0.0f, -1.0f);
        
    m_acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
    m_currentVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
    m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);

    m_orientation = glm::quat();
    m_savedOrientation = glm::quat();

    m_viewMatrix = glm::mat4(1.0f);
    m_projMatrix = glm::mat4(1.0f);
    m_viewProjMatrix = glm::mat4(1.0f);
}

Camera::~Camera()
{
}

void Camera::lookAt(const glm::vec3 &target)
{
    lookAt(m_eye, target, m_yAxis);
}

void Camera::lookAt(const glm::vec3 &eye, const glm::vec3 &target, const glm::vec3 &up)
{
    m_viewMatrix = glm::lookAt(eye, target, up);

    m_eye = eye;
    m_target = target;
    m_zAxis = glm::normalize(eye - target);
    m_viewDir = -m_zAxis;
    m_xAxis = glm::normalize(glm::cross(up, m_zAxis));
    m_yAxis = glm::normalize(glm::cross(m_zAxis, m_xAxis));
    
    // Extract the pitch angle from the view matrix.
    m_accumPitchDegrees = glm::degrees(asinf(m_viewMatrix[1][2]));
    
    m_orientation = glm::quat_cast(m_viewMatrix);
	updateViewMatrix();
}

void Camera::move(float dx, float dy, float dz)
{
    if (m_behavior == CAMERA_BEHAVIOR_ORBIT)
    {
        return;
    }

    glm::vec3 eye = m_eye;
    glm::vec3 forwards;

    if (m_behavior == CAMERA_BEHAVIOR_FIRST_PERSON)
    {
        forwards = glm::cross(WORLD_YAXIS, m_xAxis);
        forwards = glm::normalize(forwards);
    }
    else
    {
        forwards = m_viewDir;
    }

    eye += m_xAxis * dx;
    eye += WORLD_YAXIS * dy;
    eye += forwards * dz;

    setPosition(eye);
}

void Camera::move(const glm::vec3 &direction, const glm::vec3 &amount)
{
    if (m_behavior == CAMERA_BEHAVIOR_ORBIT)
    {
        return;
    }

    m_eye.x += direction.x * amount.x;
    m_eye.y += direction.y * amount.y;
    m_eye.z += direction.z * amount.z;

    updateViewMatrix();
}

void Camera::perspective(float fovx, float aspect, float znear, float zfar)
{
    float fovy = glm::degrees(2.0f * atanf(tanf(glm::radians(fovx) / 2.0f) / aspect));
    m_projMatrix = glm::perspective(glm::radians(fovy), aspect, znear, zfar);

    m_viewProjMatrix = m_projMatrix * m_viewMatrix;
    
    m_fovx = fovx;
    m_aspectRatio = aspect;
    m_znear = znear;
    m_zfar = zfar;
}

void Camera::rotate(float headingDegrees, float pitchDegrees, float rollDegrees)
{
    // Rotates the camera based on its current behavior.
    // Note that not all behaviors support rolling.

    pitchDegrees = -pitchDegrees;
    headingDegrees = -headingDegrees;
    rollDegrees = -rollDegrees;

    switch (m_behavior)
    {
    default:
        break;

    case CAMERA_BEHAVIOR_FIRST_PERSON:
    case CAMERA_BEHAVIOR_SPECTATOR:
        rotateFirstPerson(headingDegrees, pitchDegrees);
        break;

    case CAMERA_BEHAVIOR_FLIGHT:
        rotateFlight(headingDegrees, pitchDegrees, rollDegrees);
        break;

    case CAMERA_BEHAVIOR_ORBIT:
        rotateOrbit(headingDegrees, pitchDegrees, rollDegrees);
        break;
    }

    updateViewMatrix();
}

void Camera::rotateSmoothly(float headingDegrees, float pitchDegrees, float rollDegrees)
{
    // This method applies a scaling factor to the rotation angles prior to
    // using these rotation angles to rotate the camera. This method is usually
    // called when the camera is being rotated using an input device (such as a
    // mouse or a joystick). 

    headingDegrees *= m_rotationSpeed;
    pitchDegrees *= m_rotationSpeed;
    rollDegrees *= m_rotationSpeed;

    rotate(headingDegrees, pitchDegrees, rollDegrees);
}

void Camera::undoRoll()
{
    // Undo any camera rolling by leveling the camera. When the camera is
    // orbiting this method will cause the camera to become level with the
    // orbit target.

    if (m_behavior == CAMERA_BEHAVIOR_ORBIT)
        lookAt(m_eye, m_target, m_targetYAxis);
    else
        lookAt(m_eye, m_eye + m_viewDir, WORLD_YAXIS);
}

void Camera::updatePosition(const glm::vec3 &direction, float elapsedTimeSec)
{
    if (glm::length(m_currentVelocity) * glm::length(m_currentVelocity) != 0.0f)
    {
        glm::vec3 displacement = (m_currentVelocity * elapsedTimeSec) +
            (0.5f * m_acceleration * elapsedTimeSec * elapsedTimeSec);

        if (direction.x == 0.0f && glm::abs(m_currentVelocity.x) < 1e-6)
            displacement.x = 0.0f;

        if (direction.y == 0.0f && glm::abs(m_currentVelocity.y) < 1e-6)
            displacement.y = 0.0f;

        if (direction.z == 0.0f && glm::abs(m_currentVelocity.z) < 1e-6)
            displacement.z = 0.0f;

        move(displacement.x, displacement.y, displacement.z);
    }

    updateVelocity(direction, elapsedTimeSec);
}

void Camera::zoom(float zoom, float minZoom, float maxZoom)
{
    if (m_behavior == CAMERA_BEHAVIOR_ORBIT)
    {
        m_orbitMaxZoom = maxZoom;
        m_orbitMinZoom = minZoom;

        glm::vec3 offset = m_eye - m_target;

        m_orbitOffsetDistance = glm::length(offset);
        offset = glm::normalize(offset);
        m_orbitOffsetDistance += zoom;
        m_orbitOffsetDistance = std::min(std::max(m_orbitOffsetDistance, minZoom), maxZoom);

        offset *= m_orbitOffsetDistance;
        m_eye = offset + m_target;
        
        updateViewMatrix();
    }
    else
    {
        zoom = std::min(std::max(zoom, minZoom), maxZoom);
        perspective(zoom, m_aspectRatio, m_znear, m_zfar);
    }
}

void Camera::setAcceleration(const glm::vec3 &acceleration)
{
    m_acceleration = acceleration;
}

void Camera::setBehavior(CameraBehavior newBehavior)
{
    // Switch to a new camera mode (i.e., behavior).
    // This method is complicated by the fact that it tries to save the current
    // behavior's state prior to making the switch to the new camera behavior.
    // Doing this allows seamless switching between camera behaviors.

    CameraBehavior prevBehavior = m_behavior;

    if (prevBehavior == newBehavior)
        return;

    m_behavior = newBehavior;

    switch (newBehavior)
    {
    case CAMERA_BEHAVIOR_FIRST_PERSON:
        switch (prevBehavior)
        {
        default:
            break;

        case CAMERA_BEHAVIOR_FLIGHT:
            m_eye.y = m_firstPersonYOffset;
            updateViewMatrix();
            break;

        case CAMERA_BEHAVIOR_SPECTATOR:
            m_eye.y = m_firstPersonYOffset;
            updateViewMatrix();
            break;

        case CAMERA_BEHAVIOR_ORBIT:
            m_eye.x = m_savedEye.x;
            m_eye.z = m_savedEye.z;
            m_eye.y = m_firstPersonYOffset;
            m_orientation = m_savedOrientation;
            m_accumPitchDegrees = m_savedAccumPitchDegrees;
            updateViewMatrix();
            break;
        }

        undoRoll();
        break;

    case CAMERA_BEHAVIOR_SPECTATOR:
        switch (prevBehavior)
        {
        default:
            break;

        case CAMERA_BEHAVIOR_FLIGHT:
            updateViewMatrix();
            break;

        case CAMERA_BEHAVIOR_ORBIT:
            m_eye = m_savedEye;
            m_orientation = m_savedOrientation;
            m_accumPitchDegrees = m_savedAccumPitchDegrees;
            updateViewMatrix();
            break;
        }

        undoRoll();
        break;
    
    case CAMERA_BEHAVIOR_FLIGHT:
        if (prevBehavior == CAMERA_BEHAVIOR_ORBIT)
        {
            m_eye = m_savedEye;
            m_orientation = m_savedOrientation;
            m_accumPitchDegrees = m_savedAccumPitchDegrees;
            updateViewMatrix();
        }
        else
        {
            m_savedEye = m_eye;
            updateViewMatrix();
        }
        break;
    
    case CAMERA_BEHAVIOR_ORBIT:
        if (prevBehavior == CAMERA_BEHAVIOR_FIRST_PERSON)
            m_firstPersonYOffset = m_eye.y;

        m_savedEye = m_eye;
        m_savedOrientation = m_orientation;
        m_savedAccumPitchDegrees = m_accumPitchDegrees;
        
        m_targetYAxis = m_yAxis;

        glm::vec3 newEye = m_eye + m_zAxis * m_orbitOffsetDistance;
        glm::vec3 newTarget = m_eye;
        
        lookAt(newEye, newTarget, m_targetYAxis);
        break;
    }
}

void Camera::setCurrentVelocity(const glm::vec3 &currentVelocity)
{
    m_currentVelocity = currentVelocity;
}

void Camera::setCurrentVelocity(float x, float y, float z)
{
    m_currentVelocity = glm::vec3(x, y, z);
}

void Camera::setOrbitMaxZoom(float orbitMaxZoom)
{
    m_orbitMaxZoom = orbitMaxZoom;
}

void Camera::setOrbitMinZoom(float orbitMinZoom)
{
    m_orbitMinZoom = orbitMinZoom;
}

void Camera::setOrbitOffsetDistance(float orbitOffsetDistance)
{
    m_orbitOffsetDistance = orbitOffsetDistance;
}

void Camera::setOrientation(const glm::quat &newOrientation)
{
    glm::mat4 m = glm::mat4_cast(newOrientation);

    m_accumPitchDegrees = glm::degrees(asinf(m[1][2]));

    m_orientation = newOrientation;

    if (m_behavior == CAMERA_BEHAVIOR_FIRST_PERSON || m_behavior == CAMERA_BEHAVIOR_SPECTATOR)
        lookAt(m_eye, m_eye + m_viewDir, WORLD_YAXIS);

    updateViewMatrix();
}

void Camera::setPosition(const glm::vec3 &newEye)
{
    m_eye = newEye;
    updateViewMatrix();
}

void Camera::setPreferTargetYAxisOrbiting(bool preferTargetYAxisOrbiting)
{
    // Determines the behavior of Y axis rotations when the camera is
    // orbiting a target. When preferTargetYAxisOrbiting is true all
    // Y axis rotations are about the orbit target's local Y axis.
    // When preferTargetYAxisOrbiting is false then the camera's
    // local Y axis is used instead.

    m_preferTargetYAxisOrbiting = preferTargetYAxisOrbiting;

    if (m_preferTargetYAxisOrbiting)
        undoRoll();
}

void Camera::setRotationSpeed(float rotationSpeed)
{
    // This is just an arbitrary value used to scale rotations
    // when rotateSmoothly() is called.

    m_rotationSpeed = rotationSpeed;
}

void Camera::setVelocity(const glm::vec3 &velocity)
{
    m_velocity = velocity;
}

void Camera::setVelocity(float x, float y, float z)
{
    m_velocity = glm::vec3(x, y, z);
}

void Camera::rotateFirstPerson(float headingDegrees, float pitchDegrees)
{
    m_accumPitchDegrees += pitchDegrees;

    if (m_accumPitchDegrees > 90.0f)
    {
        pitchDegrees = 90.0f - (m_accumPitchDegrees - pitchDegrees);
        m_accumPitchDegrees = 90.0f;
    }

    if (m_accumPitchDegrees < -90.0f)
    {
        pitchDegrees = -90.0f - (m_accumPitchDegrees - pitchDegrees);
        m_accumPitchDegrees = -90.0f;
    }
   
    glm::quat rot;

    if (headingDegrees != 0.0f)
    {
        rot = glm::angleAxis(glm::radians(headingDegrees), WORLD_YAXIS);
        m_orientation = rot * m_orientation;
    }

    if (pitchDegrees != 0.0f)
    {
        rot = glm::angleAxis(glm::radians(pitchDegrees), WORLD_XAXIS);
        m_orientation = m_orientation * rot;
    }
}

void Camera::rotateFlight(float headingDegrees, float pitchDegrees, float rollDegrees)
{
    m_accumPitchDegrees += pitchDegrees;

    if (m_accumPitchDegrees > 360.0f)
        m_accumPitchDegrees -= 360.0f;

    if (m_accumPitchDegrees < -360.0f)
        m_accumPitchDegrees += 360.0f;
   
    glm::quat rot = glm::quat(glm::vec3(glm::radians(pitchDegrees), glm::radians(headingDegrees), glm::radians(rollDegrees)));
    m_orientation *= rot;
}

void Camera::rotateOrbit(float headingDegrees, float pitchDegrees, float rollDegrees)
{
    glm::quat rot;

    if (m_preferTargetYAxisOrbiting)
    {
        if (headingDegrees != 0.0f)
        {
            rot = glm::angleAxis(glm::radians(headingDegrees), m_targetYAxis);
            m_orientation = rot * m_orientation;
        }

        if (pitchDegrees != 0.0f)
        {
            rot = glm::angleAxis(glm::radians(pitchDegrees), WORLD_XAXIS);
            m_orientation = m_orientation * rot;
        }
    }
    else
    {
        rot = glm::quat(glm::vec3(glm::radians(pitchDegrees), glm::radians(headingDegrees), glm::radians(rollDegrees)));
        m_orientation *= rot;
    }
}

void Camera::updateVelocity(const glm::vec3 &direction, float elapsedTimeSec)
{
    // Updates the camera's velocity based on the supplied movement direction
    // and the elapsed time (since this method was last called). The movement
    // direction is in the range [-1,1].

    if (direction.x != 0.0f)
    {
        // Camera is moving along the x axis.
        // Linearly accelerate up to the camera's max speed.

        m_currentVelocity.x += direction.x * m_acceleration.x * elapsedTimeSec;

        if (m_currentVelocity.x > m_velocity.x)
            m_currentVelocity.x = m_velocity.x;
        else if (m_currentVelocity.x < -m_velocity.x)
            m_currentVelocity.x = -m_velocity.x;
    }
    else
    {
        // Camera is no longer moving along the x axis.
        // Linearly decelerate back to stationary state.

        if (m_currentVelocity.x > 0.0f)
        {
            if ((m_currentVelocity.x -= m_acceleration.x * elapsedTimeSec) < 0.0f)
                m_currentVelocity.x = 0.0f;
        }
        else
        {
            if ((m_currentVelocity.x += m_acceleration.x * elapsedTimeSec) > 0.0f)
                m_currentVelocity.x = 0.0f;
        }
    }

    if (direction.y != 0.0f)
    {
        // Camera is moving along the y axis.
        // Linearly accelerate up to the camera's max speed.

        m_currentVelocity.y += direction.y * m_acceleration.y * elapsedTimeSec;
        
        if (m_currentVelocity.y > m_velocity.y)
            m_currentVelocity.y = m_velocity.y;
        else if (m_currentVelocity.y < -m_velocity.y)
            m_currentVelocity.y = -m_velocity.y;
    }
    else
    {
        // Camera is no longer moving along the y axis.
        // Linearly decelerate back to stationary state.

        if (m_currentVelocity.y > 0.0f)
        {
            if ((m_currentVelocity.y -= m_acceleration.y * elapsedTimeSec) < 0.0f)
                m_currentVelocity.y = 0.0f;
        }
        else
        {
            if ((m_currentVelocity.y += m_acceleration.y * elapsedTimeSec) > 0.0f)
                m_currentVelocity.y = 0.0f;
        }
    }

    if (direction.z != 0.0f)
    {
        // Camera is moving along the z axis.
        // Linearly accelerate up to the camera's max speed.

        m_currentVelocity.z += direction.z * m_acceleration.z * elapsedTimeSec;

        if (m_currentVelocity.z > m_velocity.z)
            m_currentVelocity.z = m_velocity.z;
        else if (m_currentVelocity.z < -m_velocity.z)
            m_currentVelocity.z = -m_velocity.z;
    }
    else
    {
        // Camera is no longer moving along the z axis.
        // Linearly decelerate back to stationary state.

        if (m_currentVelocity.z > 0.0f)
        {
            if ((m_currentVelocity.z -= m_acceleration.z * elapsedTimeSec) < 0.0f)
                m_currentVelocity.z = 0.0f;
        }
        else
        {
            if ((m_currentVelocity.z += m_acceleration.z * elapsedTimeSec) > 0.0f)
                m_currentVelocity.z = 0.0f;
        }
    }
}

void Camera::updateViewMatrix()
{
    m_viewMatrix = glm::mat4_cast(m_orientation);

    m_xAxis = glm::vec3(m_viewMatrix[0][0], m_viewMatrix[1][0], m_viewMatrix[2][0]);
    m_yAxis = glm::vec3(m_viewMatrix[0][1], m_viewMatrix[1][1], m_viewMatrix[2][1]);
    m_zAxis = glm::vec3(m_viewMatrix[0][2], m_viewMatrix[1][2], m_viewMatrix[2][2]);
    m_viewDir = -m_zAxis;

    if (m_behavior == CAMERA_BEHAVIOR_ORBIT)
    {
        m_eye = m_target + m_zAxis * m_orbitOffsetDistance;
    }

    m_viewMatrix[3][0] = -glm::dot(m_xAxis, m_eye);
    m_viewMatrix[3][1] = -glm::dot(m_yAxis, m_eye);
    m_viewMatrix[3][2] = -glm::dot(m_zAxis, m_eye);
}