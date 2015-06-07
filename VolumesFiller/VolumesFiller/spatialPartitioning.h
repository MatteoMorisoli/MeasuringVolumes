//
//  spatialPartitioning.h
//  Volumes
//
//  Created by Matteo Morisoli on 24/04/15.
//  Copyright (c) 2015 Matteo Morisoli. All rights reserved.
//

#ifndef __VolumesFiller__spatialPartitioning__
#define __VolumesFiller__spatialPartitioning__

#include <stdio.h>
#include "glm.hpp"
#include <vector>
#include <set>

struct Node {
    std::vector<glm::vec3> triangles;
    std::vector<glm::vec3> spheres;
    std::vector<int> spheresIndices;
    std::set<int> eliminatedIndices;
    bool hasTriangles = false;
} ;

Node * createNodeGrid(std::vector<glm::vec3> vertices, std::vector<glm::vec3> spheres, double gridSize, int divisor, double sphereRadius);

int checkPointInGrid( double minimalValue, double increment, int divisor, glm::vec3 point);

std::vector<int> checkSphereInGrid( double minimalValue, double increment, double sphereRadius, int divisor, glm::vec3 point);

std::vector<glm::vec3> removeIntersectedSpheres(std::set<int> intersectedSpheresIndices, std::vector<glm::vec3> spheres);

void checkIntersectionPartitioned( Node & node, double sphere_radius, double maxSide);

std::set<int> mergeEliminatedSpheres(Node * nodes, int nodesLength);

#endif /* defined(__VolumesFiller__spatialPartitioning__) */
