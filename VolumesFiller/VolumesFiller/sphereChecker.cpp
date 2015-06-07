//
//  sphereChecker.cpp
//  Volumes
//
//  Created by Matteo Morisoli on 28/03/15.
//  Copyright (c) 2015 Matteo Morisoli. All rights reserved.
//

#include "sphereChecker.h"
//this file contains the two main algorithm for sphere removal, in single and multithreaded form form for the outside algorithm, and in single threaded form for the inside algorithm, the multithreaded form of the inside removal algorithm can be found in spatialpartitioning.cpp as it needs the node struct to work

/*algorithm to remove outside spheres, not multithreaded*/
std::vector<glm::vec3> checkOutsideSpheresLine( std::vector<std::vector<std::vector<glm::vec3>>> spheres, std::vector<glm::vec3> vertices, double maxSide){
    double  xRayCoords = 3.0;
    double YLineCoords;
    double ZLineCoords;
    glm::vec3 startPos;
    glm::vec3 normal;
    std::vector<glm::vec3> insideSpheres;
    std::vector<glm::vec3> intersectionPoints;
    glm::vec3 ray = glm::vec3( -1.0, 0.0, 0.0);
    int counter = 0;
    int debugcounter1 = 0;
    int debugcounter2 = 0;
    for(int a = 0; a < spheres.size(); a++){
        for(int b = 0; b < spheres[a].size(); b++){
            intersectionPoints = *new std::vector<glm::vec3>;
            YLineCoords = spheres[a][b][0].y;
            ZLineCoords = spheres[a][b][0].z;
            startPos = glm::vec3(xRayCoords, YLineCoords, ZLineCoords);
            for( int c = 0; c < vertices.size(); c += 3){
                if((glm::abs(vertices[c].z - ZLineCoords) <= maxSide) && (glm::abs(vertices[c].y - YLineCoords) <= maxSide) && (glm::abs(vertices[c+1].z - ZLineCoords) <= maxSide) && (glm::abs(vertices[c+1].y - YLineCoords) <= maxSide) && (glm::abs(vertices[c+2].z - ZLineCoords) <= maxSide) && (glm::abs(vertices[c+2].y - YLineCoords) <= maxSide)){
                    normal = glm::cross(vertices[c + 1] - vertices[c], vertices[c + 2] - vertices[c]);
                    double firstDot = glm::dot(normal, ray);
                    if( glm::abs(firstDot) >= 0){
                        double t = glm::dot((vertices[c] - startPos), normal) / firstDot;
                        if( t >= 0){
                            glm::vec3 intersectionPoint = startPos + ray * (float)t;
                            glm::vec3 firstBar = glm::cross(vertices[c + 1] - vertices[c], intersectionPoint - vertices[c]);
                            glm::vec3 secondBar = glm::cross(vertices[c + 2] - vertices[c + 1], intersectionPoint - vertices[c + 1]);
                            glm::vec3 thirdBar = glm::cross(vertices[c] - vertices[c + 2], intersectionPoint - vertices[c+ 2]);
                            double firstDot = glm::dot(firstBar, secondBar);
                            double secondDot = glm::dot(secondBar, thirdBar);
                            double thirdDot = glm::dot(firstBar, thirdBar);
                            
                            if(debugcounter1 == 92 && debugcounter2 == 20123){
                                
//                                printf("firstDot %.30f seconddot %.30f thirddot %.30f \n", firstDot, secondDot, thirdDot);
                            }
                            if((firstDot >= 0) && (secondDot >= 0) && (thirdDot >= 0)){
                                if(std::find(intersectionPoints.begin(), intersectionPoints.end(), intersectionPoint) == intersectionPoints.end()){
                                    intersectionPoints.push_back(intersectionPoint);
                                    counter++;
                                }
                                
                            }
                        }
                    }
                }
            }
            if(intersectionPoints.size() != 0){
                if(intersectionPoints.size() % 2 != 0){
//                    printf("mistake at %d %d intersections %d \n", debugcounter1, debugcounter2, intersectionPoints.size());
                    //printf(" firstdot %.50f seconddot %f thirddot %f \n", firstDot, secondDot, thirdDot);
                }
                std::sort(intersectionPoints.begin(), intersectionPoints.end(), [](const glm::vec3& a, const glm::vec3& b) {
                    return a.x > b.x;});
                for(int d = 0; d < spheres[a][b].size(); d++){
                    for(int e = 0; e < intersectionPoints.size(); e += 2){
                        if((spheres[a][b][d].x >= intersectionPoints[e + 1].x) && (spheres[a][b][d].x <= intersectionPoints[e].x)){
                            insideSpheres.push_back(spheres[a][b][d]);
                        }
                    }
                }
            }
            debugcounter2++;
        }
        debugcounter1++;
    }
    return insideSpheres;
}

/* algorithm for removing outside spheres, multithreaded version, each thread runs this on a plane of spheres, shoots a ray inside the  mesh, computes intersections, then takes only spheres that are between intersections. */
void checkOutsideSpheresLineThreaded( std::vector<std::vector<glm::vec3>> spheres, std::vector<glm::vec3> vertices, double maxSide, std::vector<glm::vec3> & returnSpheres){
    double  xRayCoords = 6.0;
    double YLineCoords;
    double ZLineCoords;
    glm::vec3 startPos;
    glm::vec3 normal;
    std::vector<glm::vec3> intersectionPoints;
    glm::vec3 ray = glm::vec3( -1.0, 0.0, 0.0);
    int counter = 0;
    int debugcounter = 0;
    for(int b = 0; b < spheres.size(); b++){
        intersectionPoints = *new std::vector<glm::vec3>;
        YLineCoords = spheres[b][0].y;
        ZLineCoords = spheres[b][0].z;
        startPos = glm::vec3(xRayCoords, YLineCoords, ZLineCoords);
        for( int c = 0; c < vertices.size(); c += 3){
            if((glm::abs(vertices[c].z - ZLineCoords) <= maxSide) && (glm::abs(vertices[c].y - YLineCoords) <= maxSide) && (glm::abs(vertices[c+1].z - ZLineCoords) <= maxSide) && (glm::abs(vertices[c+1].y - YLineCoords) <= maxSide) && (glm::abs(vertices[c+2].z - ZLineCoords) <= maxSide) && (glm::abs(vertices[c+2].y - YLineCoords) <= maxSide)){
                normal = glm::cross(vertices[c + 1] - vertices[c], vertices[c + 2] - vertices[c]);
                double firstDot = glm::dot(normal, ray);
                if( glm::abs(firstDot) >= 0){
                    double t = glm::dot((vertices[c] - startPos), normal) / firstDot;
                    if( t >= 0){
                        glm::vec3 intersectionPoint = startPos + ray * (float)t;
                        glm::vec3 firstBar = glm::cross(vertices[c + 1] - vertices[c], intersectionPoint - vertices[c]);
                        glm::vec3 secondBar = glm::cross(vertices[c + 2] - vertices[c + 1], intersectionPoint - vertices[c + 1]);
                        glm::vec3 thirdBar = glm::cross(vertices[c] - vertices[c + 2], intersectionPoint - vertices[c+ 2]);
                        double firstDot = glm::dot(firstBar, secondBar);
                        double secondDot = glm::dot(secondBar, thirdBar);
                        double thirdDot = glm::dot(firstBar, thirdBar);
                        if((firstDot >= glm::pow(-10.0, -13.0)) && (secondDot >= glm::pow(-10.0, -13.0)) && (thirdDot >= glm::pow(-10.0, -13.0))){
                            if(std::find(intersectionPoints.begin(), intersectionPoints.end(), intersectionPoint) == intersectionPoints.end()){
                                intersectionPoints.push_back(intersectionPoint);
                                counter++;
                            }
                            
                        }
                    }
                }
            }
        }
        if(intersectionPoints.size() != 0){
            std::sort(intersectionPoints.begin(), intersectionPoints.end(), [](const glm::vec3& a, const glm::vec3& b) {
                return a.x > b.x;});
            for(int d = 0; d < spheres[b].size(); d++){
                for(int e = 0; e < intersectionPoints.size(); e += 2){
                    if((spheres[b][d].x >= intersectionPoints[e + 1].x) && (spheres[b][d].x <= intersectionPoints[e].x)){
                        returnSpheres.push_back(spheres[b][d]);
                    }
                }
            }
        }
        debugcounter++;
    }
}


/*algorithm to remove spheres that collide with mesh, not multithreaded */
std::vector<glm::vec3> checkIntersection( std::vector<glm::vec3> spheres, std::vector<glm::vec3> vertices, double sphereRadius, double maxSide){
    std::set<int> intersectedSpheres;
    printf("number of spheres before collision check: %lu \n", spheres.size());
    for( int i = 0; i < vertices.size(); i+=3){
        for(int j = 0; j < spheres.size(); j++){
            if( (glm::length(vertices[i] - spheres[j]) >= maxSide + sphereRadius) || (glm::length(vertices[i + 1] - spheres[j]) >= maxSide + sphereRadius)){
                continue;
            }
            if(glm::distance(spheres[j], vertices[i + 2]) <= sphereRadius){
                intersectedSpheres.insert(j);
                continue;
            }
            if(glm::distance(spheres[j], vertices[i + 1]) <= sphereRadius){
                intersectedSpheres.insert(j);
                continue;
            }
            if( glm::distance(spheres[j], vertices[i]) <= sphereRadius){
                intersectedSpheres.insert(j);
                continue;
            }
            double dist1 = glm::length(glm::cross((vertices[i + 1] - vertices[i]), (spheres[j] - vertices[i]))) / glm::length((vertices[i + 1] - vertices[i]));
            if( dist1 <= sphereRadius){
                if( glm::dot((vertices[i + 1] - vertices[i]), (spheres[j] - vertices[i])) >= 0 && glm::dot((vertices[i] - vertices[i + 1]), (spheres[j] - vertices[i + 1])) >= 0){
                    intersectedSpheres.insert(j);
                    continue;
                }
            }
            double dist2 = glm::length(glm::cross((vertices[i + 2] - vertices[i + 1]), (spheres[j] - vertices[i + 1]))) / glm::length((vertices[i + 2] - vertices[i + 1]));
            if(dist2 <= sphereRadius){
                if(glm::dot((vertices[i + 2] - vertices[i + 1]), (spheres[j] - vertices[i + 1])) >= 0 && glm::dot((vertices[i + 1] - vertices[i + 2]), (spheres[j] - vertices[i + 2])) >= 0){
                    intersectedSpheres.insert(j);
                    continue;
                }
            }
            double dist3 = glm::length(glm::cross((vertices[i] - vertices[i + 2]), (spheres[j] - vertices[i + 2]))) / glm::length((vertices[i] - vertices[i + 2]));
            if(dist3 <= sphereRadius){
                if(glm::dot((vertices[i] - vertices[i + 2]), (spheres[j] - vertices[i + 2])) >= 0 && glm::dot((vertices[i + 2] - vertices[i]), (spheres[j] - vertices[i])) >= 0){
                    intersectedSpheres.insert(j);
                    continue;
                }
            }
            glm::vec3 norm = glm::cross((vertices[i + 2] - vertices[i]), (vertices[i + 1] - vertices[i]));
            double normLength = glm::length(norm);
            double planeDistance = glm::dot((spheres[j] - vertices[i]), norm) / normLength;
            if( glm::abs(planeDistance) <= sphereRadius){
                glm::vec3 firstBar = glm::cross(spheres[j] - vertices[i + 1], spheres[j] - vertices[i + 2]);
                glm::vec3 secondBar = glm::cross(spheres[j] - vertices[i + 2], spheres[j] - vertices[i]);
                glm::vec3 thirdBar = glm::cross(spheres[j] - vertices[i], spheres[j] - vertices[i + 1]);
                double firstDot = glm::dot(norm, firstBar);
                double secondDot = glm::dot(norm, secondBar);
                double thirdDot = glm::dot(norm, thirdBar);
                if((firstDot <= 0) && (secondDot <= 0) && (thirdDot <= 0)){
                    intersectedSpheres.insert(j);
                    
                }
            }
        }
    }
    std::vector<glm::vec3> filteredSpheres;
    for(int k = 0; k < spheres.size(); k++){
        if(intersectedSpheres.find(k) == intersectedSpheres.end()){
            filteredSpheres.push_back(spheres[k]);
        }
    }
    return filteredSpheres;
}