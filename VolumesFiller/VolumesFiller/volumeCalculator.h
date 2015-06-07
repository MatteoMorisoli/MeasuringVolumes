//
//  volumeCalculator.h
//  Volumes
//
//  Created by Matteo Morisoli on 12/04/15.
//  Copyright (c) 2015 Matteo Morisoli. All rights reserved.
//

#ifndef __VolumesFiller__volumeCalculator__
#define __VolumesFiller__volumeCalculator__

#include <stdio.h>
#include "glm.hpp"
#include <vector>
#include <math.h>

double calculateSphereVolumes( double sphereRadius, unsigned long sphereNumber);

double calculateObjectVolume(std::vector<glm::vec3> vertices);

double calculateFillingPercentage(double objectVolume, double spheresVolume);
#endif /* defined(__VolumesFiller__volumeCalculator__) */
