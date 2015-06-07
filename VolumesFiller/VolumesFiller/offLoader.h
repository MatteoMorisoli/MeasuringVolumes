//
//  OffLoader.h
//  Volumes
//
//  Created by Matteo Morisoli on 11/03/15.
//  Copyright (c) 2015 Matteo Morisoli. All rights reserved.
//

#ifndef __VolumesFiller__offLoader__
#define __VolumesFiller__offLoader__

#include <stdio.h>
#include <stdlib.h>
#include "glm.hpp"
#include <vector>
#include <string>
#include <math.h>
#include <iostream>
#include <fstream>
#include "matrix_transform.hpp"


void loadOFFComplex(
                    const char * path,
                    std::vector<glm::vec3> & outVerticesZ,
                    std::vector<glm::vec3> & outNormalsZ,
                    double & maxSide
                    );

void loadOFF(
             const char * path,
             std::vector<glm::vec3> & outVertices,
             std::vector<glm::vec3> & outNormals
             );

void convertOFF(const char * path);
#endif /* defined(__VolumesFiller__offLoader__) */
