#include "scene/Moon.h"
#include "utils/Log.h"

#include <cmath>
#include <string>

// Constructors
Moon::Moon(std::string objectName) : PlanetaryObject(objectName)
{
    InitializeObject();
}


Moon::~Moon() = default;


void  Moon::InitializeObject(void)
{
    SetObjectRadius(0.50);
    SetObjectAxisTilt(0.0);
    
    m_moonCurrentOrbitalAngle = 180.0;
    
    // Initialize the position
    GLfloat orbitRadius = (ASTRO_MATH_LIB::KMtoGU(384000 + EARTH_RADIUS_KM));
    
    if (Log::kDebugLoggingEnabled)
    {
        Log::debug(std::string("Moon orbit angle: ") +
                   std::to_string(m_moonCurrentOrbitalAngle));
        Log::debug(std::string("Moon orbit radius: ") + std::to_string(orbitRadius));
    }
    
    m_moonPositionX = orbitRadius * cos(m_moonCurrentOrbitalAngle);
    m_moonPositionZ = orbitRadius * sin(m_moonCurrentOrbitalAngle);
    m_moonPositionY = 0.0;
}


void Moon::UpdateObject(void)
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
    
    if (Log::kDebugLoggingEnabled)
    {
        Log::debug(std::string("Moon orbit angle: ") +
                   std::to_string(m_moonCurrentOrbitalAngle));
        Log::debug(std::string("Moon orbit radius: ") + std::to_string(orbitRadius));
    }
    
    m_moonPositionX = orbitRadius * cos(m_moonCurrentOrbitalAngle);
    m_moonPositionZ = orbitRadius * sin(m_moonCurrentOrbitalAngle);
    m_moonPositionY = 0.0;
}