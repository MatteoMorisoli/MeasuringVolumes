//
//  spheresPos.cpp
//  Volumes
//
//  Created by Matteo Morisoli on 27/03/15.
//  Copyright (c) 2015 Matteo Morisoli. All rights reserved.
//

#include "spheresPos.h"


/*function that generates a sphere lattice following the hexagonal pattern, with the given size and the given sphere radius */
std::vector<std::vector<std::vector<glm::vec3>>> generateSpherePosABA(double gridSize, double radius){
    std::vector<glm::vec3> sphereCenters;
    
    std::vector<std::vector<std::vector<glm::vec3>>> spheresLists;
    std::vector<std::vector<glm::vec3>> spheresList;
    double zHeigth = glm::sqrt(3) * radius;
    double yOffset = glm::sqrt(3) / 3.0 * radius;
    double yHeigth = glm::cos( glm::asin(yOffset / zHeigth)) * zHeigth;
    double startX = (-1 * gridSize) / 2.0;
    double startY = (-1 * gridSize) / 2.0;
    double startZ = (-1 * gridSize) / 2.0;
    
    int verticalLineCounter = 0;
    double actualY = startY;
    while(actualY < -1 * startY){
        double actualZ;
        int lineCounter;
        if( verticalLineCounter % 2 == 0){
            actualZ = startZ;
            lineCounter = 0;
        }else{
            actualZ = startZ + yOffset;
            lineCounter = 1;
        }
        spheresList = *new std::vector<std::vector<glm::vec3>>;
        while(actualZ < -1 * startZ){
            double actualX;
            if( lineCounter % 2 == 0){
                actualX = startX;
            }else{
                actualX = startX + radius;
            }
            sphereCenters = *new std::vector<glm::vec3>;
            while(actualX < -1 * startX){
                sphereCenters.push_back(glm::vec3(actualX, actualY, actualZ));
                actualX += 2.0 * radius;
            }
            spheresList.push_back(sphereCenters);
            lineCounter++;
            actualZ += zHeigth;
        }
        spheresLists.push_back(spheresList);
        verticalLineCounter++;
        actualY += yHeigth;
    }
    return spheresLists;
}

/*function that generates a sphere lattice following the face-centered pattern, with the given size and the given sphere radius */
std::vector<std::vector<std::vector<glm::vec3>>> generateSpherePosABC( double gridSize, double radius){
    std::vector<glm::vec3> sphereCenters;
    std::vector<std::vector<std::vector<glm::vec3>>> spheresLists;
    std::vector<std::vector<glm::vec3>> spheresList;
    double zHeigth = glm::sqrt(3) * radius;
    double yOffset = glm::sqrt(3) / 3.0 * radius;
    double yHeigth = glm::cos( glm::asin(yOffset / zHeigth)) * zHeigth;
    double startX = (-1 * gridSize) / 2.0;
    double startY = (-1 * gridSize) / 2.0;
    double startZ = (-1 * gridSize) / 2.0;
    
    int verticalLineCounter = 1;
    double actualY = startY;
    while(actualY < -1 * startY){
        double actualZ;
        int lineCounter;
        if( verticalLineCounter % 3 == 1){
            actualZ = startZ;
            lineCounter = 0;
        }else if( verticalLineCounter % 3 == 2){
            actualZ = startZ + yOffset;
            lineCounter = 1;
        }else{
            actualZ = startZ +  2 * yOffset;
            lineCounter = 2;
        }
        spheresList = *new std::vector<std::vector<glm::vec3>>;
        while(actualZ < -1 * startZ){
            double actualX;
            if( lineCounter % 2 == 0){
                actualX = startX;
            }else if(lineCounter % 2 == 1){
                actualX = startX + radius;
            }else{
                actualX = startX;
            }
            sphereCenters = *new std::vector<glm::vec3>;
            while(actualX < -1 * startX){
                sphereCenters.push_back(glm::vec3(actualX, actualY, actualZ));
                actualX += 2.0 * radius;
            }
            spheresList.push_back(sphereCenters);
            lineCounter++;
            actualZ += zHeigth;
        }
        spheresLists.push_back(spheresList);
        verticalLineCounter++;
        actualY += yHeigth;
    }
    return spheresLists;
    
}