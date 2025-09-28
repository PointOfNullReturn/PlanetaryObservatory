#ifndef PLANETARYOBSERVATORY_MOON_H
#define PLANETARYOBSERVATORY_MOON_H

#include "common/EOGL.h"

// C++ Std Includes
#include <memory>
#include <string>
#include <vector>

// Class Includes
#include "scene/PlanetaryObject.h"

class Moon : public PlanetaryObject
{
public:
    // Constructors
    Moon(std::string objectName);
    ~Moon();
    
    // Other
    void UpdateObject(void);
    
private:
    // Other Methods
    void InitializeObject(void);
    
    // Data Members
    GLfloat m_moonPositionX;
    GLfloat m_moonPositionY;
    GLfloat m_moonPositionZ;
    GLfloat m_moonCurrentOrbitalAngle;
};

#endif // PLANETARYOBSERVATORY_MOON_H