//
//  loadShaders.h
//  Volumes
//
//  Created by Matteo Morisoli on 06/03/15.
//  method loadShaders taken from http://www.opengl-tutorial.org/
//  Copyright (c) 2015 Matteo Morisoli. All rights reserved.
//

#ifndef __VolumesFiller__loadShaders__
#define __VolumesFiller__loadShaders__

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <GLFW/glfw3.h>

#include "glm.hpp"

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

#endif /* defined(__VolumesFiller__loadShaders__) */
