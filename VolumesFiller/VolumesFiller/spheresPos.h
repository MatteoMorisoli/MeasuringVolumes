//
//  spheresPos.h
//  Volumes
//
//  Created by Matteo Morisoli on 27/03/15.
//  Copyright (c) 2015 Matteo Morisoli. All rights reserved.
//

#ifndef __VolumesFiller__spheresPos__
#define __VolumesFiller__spheresPos__

#include <stdio.h>
#include "glm.hpp"
#include <vector>

std::vector<std::vector<std::vector<glm::vec3>>> generateSpherePosABA( double gridSize, double radius);

std::vector<std::vector<std::vector<glm::vec3>>> generateSpherePosABC( double gridSize, double radius);

#endif /* defined(__VolumesFiller__spheresPos__) */
