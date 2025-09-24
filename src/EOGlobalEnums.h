//
//  EOGlobalEnums.h
//  EarthObservatory
//
//  Created by Kevin Cox on 11/24/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef EarthObservatory_EOGlobalEnums_h
#define EarthObservatory_EOGlobalEnums_h

/*
--------------------------------------------------------------------------------
 Enumerations
------------------------------------------------------------------------------*/

// The mode with which to render the scene
enum RenderModes {RENDER_MODE_NORMAL, RENDER_MODE_WIREFRAME};

enum Lights { AMBIENT_LIGHT, LIGHT1, LIGHT2, LIGHT3, LIGHT4, LIGHT5, LIGHT6, LIGHT7 };

enum CameraModes { CAMERA_ORBIT, CAMERA_MOVE };

struct positionalVector {
  
    GLdouble x;
    GLdouble y;
    GLdouble z;
};

#endif
