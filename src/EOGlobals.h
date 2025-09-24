//
//  EOGlobals.h
//  EarthObservatory
//
//  Created by Kevin Cox on 11/19/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef EarthObservatory_EOGlobals_h
#define EarthObservatory_EOGlobals_h

#include "EOGL.h"

/*
--------------------------------------------------------------------------------
 Enumerations
------------------------------------------------------------------------------*/
// Screen Modes
enum ScreenModes { SCREEN_MODE_WINDOWED, SCREEN_MODE_FULLSCREEN };


/*
--------------------------------------------------------------------------------
 Computer Screen Properties
------------------------------------------------------------------------------*/
// Computer Screen Width and Height
static GLint screenWidth;
static GLint screenHeight;

// Computer Screen Mode (Default)
static ScreenModes screenMode = SCREEN_MODE_WINDOWED;

// Aspect Ratio
static GLfloat aspectRatio;

/*
--------------------------------------------------------------------------------
Screen Window Properties
------------------------------------------------------------------------------*/
// Primary Screen Window Title
static const char primaryScreenWindowTitle[] = "Earth Obervatory";

// Screen Window Position X and Y (Defaults)
static GLint screenWindowPositionX = 0;
static GLint screenWindowPositionY = 0;

// Screen Window Width and Height for HD Ratio of 16:9 (Defaults)
static GLint screenWindowWidth = 1280;
static GLint screenWindowHeight = 720;

/*
--------------------------------------------------------------------------------
 Animation Properties
------------------------------------------------------------------------------*/
// Animation Enable (Default)
static bool enableAnimation = false;


/*
--------------------------------------------------------------------------------
 Basic Viewing Properties
------------------------------------------------------------------------------*/
// Wireframe Enable/Disable (Default)
static bool enableWireframe = true;

/*
--------------------------------------------------------------------------------
 Basic Lighting Properties
------------------------------------------------------------------------------*/
// Basic Lighting
//GLfloat ambientLightColor[] = {0.50, 0.50, 0.50, 1.0};


#endif
