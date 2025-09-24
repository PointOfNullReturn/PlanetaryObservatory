//
//  EOAxis.cpp
//  EarthObservatory
//
//  Created by Kevin Cox on 11/19/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//


// Class Includes
#include "EOAxis.h"

#include <algorithm>

namespace
{
    void DrawArrowHead(GLdouble length, GLdouble width)
    {
        const GLdouble half = width * 0.5;

        glBegin(GL_TRIANGLES);
        glVertex3d(0.0, 0.0, length);
        glVertex3d(-half, -half, 0.0);
        glVertex3d(half, -half, 0.0);

        glVertex3d(0.0, 0.0, length);
        glVertex3d(half, -half, 0.0);
        glVertex3d(half, half, 0.0);

        glVertex3d(0.0, 0.0, length);
        glVertex3d(half, half, 0.0);
        glVertex3d(-half, half, 0.0);

        glVertex3d(0.0, 0.0, length);
        glVertex3d(-half, half, 0.0);
        glVertex3d(-half, -half, 0.0);
        glEnd();

        glBegin(GL_QUADS);
        glVertex3d(-half, -half, 0.0);
        glVertex3d(half, -half, 0.0);
        glVertex3d(half, half, 0.0);
        glVertex3d(-half, half, 0.0);
        glEnd();
    }
}

EOAxis::EOAxis(void)
{
    axesLength = 10.0;      // By Default, the axes length will be 10.0
    axesLineWidth = 2.0;    // By Default, the axes line width will be 2.0
    enableAxes = false;     // By Default, don't render the axes
}

EOAxis::EOAxis(GLdouble length, GLdouble lineWidth, GLboolean showAxes)
{
    axesLength = length;
    axesLineWidth = lineWidth;
    enableAxes = showAxes;
}

EOAxis::~EOAxis(void)
{
    
}

// Accessor Methods
GLboolean EOAxis::GetEnableAxes(void)
{
    return enableAxes;
}

GLdouble EOAxis::GetAxesLineWidth(void)
{
    return axesLineWidth;    
}

GLdouble EOAxis::GetAxesLength(void)
{
    return axesLength;
}

// Mutator Methods
void EOAxis::SetEnableAxes(GLboolean showAxes)
{
    enableAxes = showAxes;
}

void EOAxis::SetAxesLineWidth(GLdouble width)
{
    axesLineWidth = width;
}

void EOAxis::SetAxesLength(GLdouble length)
{
    axesLength = length;
}

// Other Methods
void EOAxis::Render(void)
{
    // Push All Attribute Bits
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    
    // Diable All Lighting
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    
    // Set the Line Width of the Axes
    glLineWidth(axesLineWidth);
    
    const GLdouble arrowLength = std::max(axesLength * 0.2, 0.2);
    const GLdouble arrowWidth = std::max(axesLength * 0.08, 0.1);

    // X-Axis Line
    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex3d(-(axesLength), 0.0f, 0.0f);
    glVertex3d(axesLength, 0.0f, 0.0f);
    glEnd();

    // X-Axis Positive Direction Arrow
    glPushMatrix();
    glTranslated(axesLength, 0.0f, 0.0f);
    glRotated(90.0f, 0.0, 1.0, 0.0);
    DrawArrowHead(arrowLength, arrowWidth);
    glPopMatrix();
    
    // Y-Axis Line
    glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex3d(0.0f, -(axesLength), 0.0f);
    glVertex3d(0.0f, axesLength, 0.0f);
    glEnd();
    
    // Y-Axis Positive Direction Cone
    glPushMatrix();
    glTranslated(0.0f, axesLength, 0.0f);
    glRotated(-90.0f, 1.0, 0.0, 0.0);
    DrawArrowHead(arrowLength, arrowWidth);
    glPopMatrix();
    
    // Z-Axis Line
    glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex3d(0.0f, 0.0f, -(axesLength));
    glVertex3d(0.0f, 0.0f, axesLength);
    glEnd();
    
    // Z-Axis Positive Direction Cone
    glPushMatrix();
    glTranslated(0.0f, 0.0f, axesLength);
    DrawArrowHead(arrowLength, arrowWidth);
    glPopMatrix();
    
    // Enable Lighting
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    
    // Pop All Attribute Bits
    glPopAttrib();
    
}