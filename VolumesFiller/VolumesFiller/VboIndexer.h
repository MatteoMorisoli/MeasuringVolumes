//
//  VboIndexer.h
//  Volumes
//
//  Created by Matteo Morisoli on 06/03/15.
//  method indexVBO taken from http://www.opengl-tutorial.org/
//  Copyright (c) 2015 Matteo Morisoli. All rights reserved.
//

#ifndef __VolumesFiller__VboIndexer__
#define __VolumesFiller__VboIndexer__

#include <stdio.h>
#include "glm.hpp"
#include <vector>
#include <map>

void indexVBO(
              std::vector<glm::vec3> & in_vertices,
              std::vector<glm::vec3> & in_normals,
              
              std::vector<unsigned short> & out_indices,
              std::vector<glm::vec3> & out_vertices,
              std::vector<glm::vec3> & out_normals
              );

#endif /* defined(__VolumesFiller__VboIndexer__) */
