#ifndef PLANETARYOBSERVATORY_EARTH_H
#define PLANETARYOBSERVATORY_EARTH_H

#include "common/EOGL.h"

// C++ Std Includes
#include <memory>
#include <string>

// Class Includes
#include "scene/PlanetaryObject.h"

class Earth : public PlanetaryObject
{
public:
    // Constructors
    Earth(std::string objectName);
    ~Earth();
    
    // Other
    void UpdateObject(void);
    
private:
    // Other Methods
    void InitializeObject(void);
    
    GLfloat m_rotationAngle;
};

#endif // PLANETARYOBSERVATORY_EARTH_H