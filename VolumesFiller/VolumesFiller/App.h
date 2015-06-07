//
//  App.h
//  VolumesFiller
//
//  Created by Matteo Morisoli on 06/06/15.
//  Copyright (c) 2015 Matteo Morisoli. All rights reserved.
//

#ifndef __VolumesFiller__App__
#define __VolumesFiller__App__

#include <stdio.h>
#include <stdlib.h>
#include <OPENGL/gl3.h>
#include "loadShaders.h"
#include <GLFW/glfw3.h>
#include "matrix_transform.hpp"
#include "glm.hpp"
#include "offLoader.h"
#include "VboIndexer.h"
#include "spheresPos.h"
#include "sphereChecker.h"
#include "volumeCalculator.h"
#include "spatialPartitioning.h"
#include <thread>
#include <time.h>
#include <chrono>


void mainLoop(double spheresRadius, double gridSize, std::string lattice, std::string mesh);

#endif /* defined(__VolumesFiller__App__) */
