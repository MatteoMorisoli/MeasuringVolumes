//
//  sphereChecker.h
//  Volumes
//
//  Created by Matteo Morisoli on 28/03/15.
//  Copyright (c) 2015 Matteo Morisoli. All rights reserved.
//

#ifndef __VolumesFiller__sphereChecker__
#define __VolumesFiller__sphereChecker__

#include <stdio.h>
#include "glm.hpp"
#include <vector>
#include <set>
//#include "spatialPartitioning.h"

std::vector<glm::vec3> checkIntersection( std::vector<glm::vec3> spheres, std::vector<glm::vec3> vertices, double sphereRadius, double maxSide);

std::vector<glm::vec3> checkOutsideSpheresLine( std::vector<std::vector<std::vector<glm::vec3>>> spheres, std::vector<glm::vec3> vertices, double maxSide);

void checkOutsideSpheresLineThreaded( std::vector<std::vector<glm::vec3>> spheres, std::vector<glm::vec3> vertices, double maxSide, std::vector<glm::vec3> & returnSpheres);

#endif /* defined(__VolumesFiller__sphereChecker__) */
