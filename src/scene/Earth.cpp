#include "scene/Earth.h"

// Constructors
Earth::Earth(std::string objectName) : PlanetaryObject(objectName)
{
    InitializeObject();

    m_rotationAngle = 0.0f;
}


Earth::~Earth() = default;


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