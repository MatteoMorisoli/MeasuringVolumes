//
//  volumeCalculator.cpp
//  Volumes
//
//  Created by Matteo Morisoli on 12/04/15.
//  Copyright (c) 2015 Matteo Morisoli. All rights reserved.
//

#include "volumeCalculator.h"
//Tool file that includes all methods relating to volume calculation and filling percentage

//calculate the volume of the approximation of spheres, parameters are the radius of the spheres and the number of spheres in the approximation
double calculateSphereVolumes( double sphereRadius, unsigned long sphereNumber){
    return (4.0 / 3.0 * glm::pow(sphereRadius, 3.0) * M_PI) * (double) sphereNumber;
}

//calculates the exact volume of the triangle mesh, the parameter is a list of triangle points
double calculateObjectVolume(std::vector<glm::vec3> vertices){
    double totalVolume = 0.0;
    for(int a = 0; a < vertices.size(); a += 3){
        double volume = glm::dot(glm::cross(vertices[a], vertices[a + 1]), vertices[a + 2]) / 6.0;
        totalVolume += volume;
    }
    return totalVolume;
}

//calculates the filling percentage of the object by the approximation of spheres, parameters are the exact volume of the object and the volume of the sphere approximation
double calculateFillingPercentage(double objectVolume, double spheresVolume){
    return spheresVolume / objectVolume * 100.0;
}