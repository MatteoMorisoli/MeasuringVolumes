//
//  spatialPartitioning.cpp
//  Volumes
//
//  Created by Matteo Morisoli on 24/04/15.
//  Copyright (c) 2015 Matteo Morisoli. All rights reserved.
//

#include "spatialPartitioning.h"

/*function that creates a node grid with given parameters, and also given a set of vertices and spheres, assigns them to the node in which they reside in the grid */
Node * createNodeGrid(std::vector<glm::vec3> vertices, std::vector<glm::vec3> spheres, double gridSize, int divisor, double sphereRadius){
    int nodesSize = divisor * divisor * divisor;
    Node * nodes = new Node[nodesSize];
    double minimalValue = gridSize / -2.0;
    double increment = gridSize / (double) divisor;
    for(int a = 0; a < vertices.size(); a += 3){
        int v1 = checkPointInGrid(minimalValue, increment, divisor, vertices[a]);
        nodes[v1].triangles.push_back(vertices[a]);
        nodes[v1].triangles.push_back(vertices[a + 1]);
        nodes[v1].triangles.push_back(vertices[a + 2]);
        nodes[v1].hasTriangles = true;
        int v2 = checkPointInGrid(minimalValue, increment, divisor, vertices[a + 1]);
        if(v2 != v1){
            nodes[v2].triangles.push_back(vertices[a]);
            nodes[v2].triangles.push_back(vertices[a + 1]);
            nodes[v2].triangles.push_back(vertices[a + 2]);
            nodes[v2].hasTriangles = true;
        }
        int v3 = checkPointInGrid(minimalValue, increment, divisor, vertices[a + 2]);
        if( (v3 != v1) && (v3 != v2)){
            nodes[v3].triangles.push_back(vertices[a]);
            nodes[v3].triangles.push_back(vertices[a + 1]);
            nodes[v3].triangles.push_back(vertices[a + 2]);
            nodes[v3].hasTriangles = true;
        }
    }
    for(int b = 0; b < spheres.size(); b++){
        std::vector<int> sphereNodes = checkSphereInGrid(minimalValue, increment, sphereRadius, divisor, spheres[b]);
        for(int c = 0; c < sphereNodes.size(); c++){
            nodes[sphereNodes[c]].spheres.push_back(spheres[b]);
            nodes[sphereNodes[c]].spheresIndices.push_back(b);
        }
    }
    return nodes;
}

/*function that checks a point and returns in which node of the grid the point resides, parameters are the parameters of the grid */
int checkPointInGrid( double minimalValue, double increment, int divisor, glm::vec3 point){
    double differenceX = point.x - minimalValue;
    double differenceY = point.y - minimalValue;
    double differenceZ = point.z - minimalValue;
    int gridX =  glm::floor(differenceX / increment);
    int gridY =  glm::floor(differenceY / increment);
    int gridZ =  glm::floor(differenceZ / increment);
    return gridX + increment * gridY + increment * increment * gridZ;
}

/*function which checks in which nodes a sphere resides, returns the array of index of the nodes that have at least part of the sphere inside them */
std::vector<int>  checkSphereInGrid( double minimalValue, double increment, double sphereRadius, int divisor, glm::vec3 point){
    std::vector<int> sphereNodes;
    double differenceX = point.x - minimalValue;
    double differenceXPlusR = point.x + sphereRadius - minimalValue;
    double differenceXMinusR = point.x - sphereRadius - minimalValue;
    double differenceY = point.y - minimalValue;
    double differenceYPlusR = point.y + sphereRadius - minimalValue;
    double differenceYMinusR = point.y - sphereRadius - minimalValue;
    double differenceZ = point.z - minimalValue;
    double differenceZPlusR = point.z + sphereRadius - minimalValue;
    double differenceZMinusR = point.z - sphereRadius - minimalValue;
    int gridX =  glm::floor(differenceX / increment);
    //printf(" %d x \n", gridX);
    int gridXPlus =  glm::floor(differenceXPlusR / increment);
    int gridXMinus =  glm::floor(differenceXMinusR / increment);
    int gridY =  glm::floor(differenceY / increment);
    //printf(" %d y \n", gridY);
    int gridYPlus =  glm::floor(differenceYPlusR / increment);
    int gridYMinus =  glm::floor(differenceYMinusR / increment);
    int gridZ =  glm::floor(differenceZ / increment);
    //printf(" %d z \n", gridZ);
    int gridZPlus =  glm::floor(differenceZPlusR / increment);
    int gridZMinus =  glm::floor(differenceZMinusR / increment);
    sphereNodes.push_back(gridX + increment * gridY + increment * increment * gridZ);
    if(gridX != gridXPlus){
        sphereNodes.push_back(gridXPlus + increment * gridY + increment * increment * gridZ);
    }
    if(gridX != gridXMinus){
        sphereNodes.push_back(gridXMinus + increment * gridY + increment * increment * gridZ);
    }
    if(gridY != gridYPlus){
        sphereNodes.push_back(gridXPlus + increment * gridYPlus + increment * increment * gridZ);
    }
    if(gridY != gridYMinus){
        sphereNodes.push_back(gridXPlus + increment * gridYMinus + increment * increment * gridZ);
    }
    if(gridZ != gridZPlus){
        sphereNodes.push_back(gridXPlus + increment * gridY + increment * increment * gridZPlus);
    }
    if(gridZ != gridZMinus){
        sphereNodes.push_back(gridXPlus + increment * gridY + increment * increment * gridZMinus);
    }
    return sphereNodes;
}

/*function that merges together all indexes of eliminated spheres from various nodes and returns the merged list */
std::set<int> mergeEliminatedSpheres(Node * nodes, int nodesLength){
    std::set<int> fullEliminatedSpheresIndices;
    for(int a =0; a < nodesLength; a++){
        for (std::set<int>::iterator b =nodes[a].eliminatedIndices.begin(); b != nodes[a].eliminatedIndices.end(); ++b) {
            int a = *b;
            fullEliminatedSpheresIndices.insert(a);
        }
    }
    return fullEliminatedSpheresIndices;
}

/*utility function that remove all the spheres with a certain index from a vector of spheres, parameters are the indices of the spheres to remove, and the list of spheres, returns the cleared list*/
std::vector<glm::vec3> removeIntersectedSpheres(std::set<int> intersectedSpheresIndices, std::vector<glm::vec3> spheres){
    std::vector<glm::vec3> correctedSpheres;
    for(int a = 0; a < spheres.size(); a++){
        if( intersectedSpheresIndices.find(a) == intersectedSpheresIndices.end()){
            correctedSpheres.push_back(spheres[a]);
        }
        
    }
    return correctedSpheres;
}

/* this algorithm removes all the spheres that intersect a triangle in the given space node, the parameters are the node, which contains the spheres and triangles present in a portion of space, the radius of the spheres, and the maximum edge length present in the triangle mesh */
void checkIntersectionPartitioned( Node & node, double sphereRadius, double maxSide){
    if( !node.hasTriangles){
        return;
    }
    //printf("number of spheres before collision check: %lu \n", node.spheres.size());
    for( int i = 0; i < node.triangles.size(); i+=3){
        for(int j = 0; j < node.spheres.size(); j++){
            if( (glm::length(node.triangles[i] - node.spheres[j]) > maxSide + sphereRadius)){
                continue;
            }
            glm::vec3 norm = glm::cross((node.triangles[i + 2] - node.triangles[i]), (node.triangles[i + 1] - node.triangles[i]));
            double normLength = glm::length(norm);
            double planeDistance = glm::dot((node.spheres[j] - node.triangles[i]), norm) / normLength;
            if( glm::abs(planeDistance) <= sphereRadius){
                glm::vec3 firstBar = glm::cross(node.spheres[j] - node.triangles[i + 1], node.spheres[j] - node.triangles[i + 2]);
                glm::vec3 secondBar = glm::cross(node.spheres[j] - node.triangles[i + 2], node.spheres[j] - node.triangles[i]);
                glm::vec3 thirdBar = glm::cross(node.spheres[j] - node.triangles[i], node.spheres[j] - node.triangles[i + 1]);
                double firstDot = glm::dot(norm, firstBar);
                double secondDot = glm::dot(norm, secondBar);
                double thirdDot = glm::dot(norm, thirdBar);
                if((firstDot <= 0) && (secondDot <= 0) && (thirdDot <= 0)){
                    node.eliminatedIndices.insert(node.spheresIndices[j]);
                    
                }
            }
            if(glm::distance(node.spheres[j], node.triangles[i + 2]) <= sphereRadius){
                node.eliminatedIndices.insert(node.spheresIndices[j]);
                continue;
            }
            if(glm::distance(node.spheres[j], node.triangles[i + 1]) <= sphereRadius){
                node.eliminatedIndices.insert(node.spheresIndices[j]);
                continue;
            }
            if( glm::distance(node.spheres[j], node.triangles[i]) <= sphereRadius){
                node.eliminatedIndices.insert(node.spheresIndices[j]);
                continue;
            }
            double dist1 = glm::length(glm::cross((node.triangles[i + 1] - node.triangles[i]), (node.spheres[j] - node.triangles[i]))) / glm::length((node.triangles[i + 1] - node.triangles[i]));
            if( dist1 <= sphereRadius){
                if( glm::dot((node.triangles[i + 1] - node.triangles[i]), (node.spheres[j] - node.triangles[i])) >= 0 && glm::dot((node.triangles[i] - node.triangles[i + 1]), (node.spheres[j] - node.triangles[i + 1])) >= 0){
                    node.eliminatedIndices.insert(node.spheresIndices[j]);
                    continue;
                }
            }
            double dist2 = glm::length(glm::cross((node.triangles[i + 2] - node.triangles[i + 1]), (node.spheres[j] - node.triangles[i + 1]))) / glm::length((node.triangles[i + 2] - node.triangles[i + 1]));
            if(dist2 <= sphereRadius){
                if(glm::dot((node.triangles[i + 2] - node.triangles[i + 1]), (node.spheres[j] - node.triangles[i + 1])) >= 0 && glm::dot((node.triangles[i + 1] - node.triangles[i + 2]), (node.spheres[j] - node.triangles[i + 2])) >= 0){
                    node.eliminatedIndices.insert(node.spheresIndices[j]);
                    continue;
                }
            }
            double dist3 = glm::length(glm::cross((node.triangles[i] - node.triangles[i + 2]), (node.spheres[j] - node.triangles[i + 2]))) / glm::length((node.triangles[i] - node.triangles[i + 2]));
            if(dist3 <= sphereRadius){
                if(glm::dot((node.triangles[i] - node.triangles[i + 2]), (node.spheres[j] - node.triangles[i + 2])) >= 0 && glm::dot((node.triangles[i + 2] - node.triangles[i]), (node.spheres[j] - node.triangles[i])) >= 0){
                    node.eliminatedIndices.insert(node.spheresIndices[j]);
                    continue;
                }
            }
        }
    }
}