//
//  OffLoader.cpp
//  Volumes
//
//  Created by Matteo Morisoli on 11/03/15.
//  Copyright (c) 2015 Matteo Morisoli. All rights reserved.
//

#include "offLoader.h"

//extended object loader for off files, this version loads the triangle mesh, does normal smoothing and sorts the point by their Z coordinate
void loadOFFComplex(
                    const char * path,
                    std::vector<glm::vec3> & outVerticesZ,
                    std::vector<glm::vec3> & outNormalsZ,
                    double & maxSide
                    ){
    printf("Loading OFF file %s...\n", path);
    /* create temporary arrays and variables*/
    int vertexNumber = 0;
    int polygonNumber = 0;
    int lineNumber = 0;
    double max = 0.0;
    std::vector<float> meanZValues;
    std::vector<float> meanYValues;
    std::vector<float> meanXValues;
    std::vector<glm::vec3> inputVertices;
    std::vector<glm::vec3> tempVertices;
    std::vector<glm::vec3> tempNormals;
    //open file
    FILE * file = fopen(path, "r");
    //file not found
    if( file == NULL ){
        printf("Impossible to open the file ");
        getchar();
        return;
    }
    
    char fileHeader[128];
    int res = fscanf(file, "%s", fileHeader);
    if (res == EOF){
        return;
        //read first line, if it is "OFF" read second line and store number of vertices and faces
    }else if(strcmp(fileHeader, "OFF\n")){
        fscanf(file, "%d %d %d\n", &vertexNumber, &polygonNumber, &lineNumber);
        //        printf( "%d %d %d\n", vertex_number, polygon_number, line_number);
    }
    std::vector<std::vector<int>> vertexUses (vertexNumber);
    //read all vertices and store them in input_vertices
    for(int i = 0; i < vertexNumber ; i++){
        glm::vec3 vertex;
        fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
        inputVertices.push_back(vertex);
    }
    //read all faces composition and put the 3 vertices in the temp_array, also mark vertices usage in vertex_uses for use in normal smoothing
    for(int j = 0; j < polygonNumber ; j++){
        int vertexCount, firstVertex, secondVertex, thirdVertex;
        fscanf(file, "%d %d %d %d\n", &vertexCount, &firstVertex, &secondVertex, &thirdVertex);
        
        //mark vertices use
        vertexUses[firstVertex].push_back(3 * j);
        vertexUses[secondVertex].push_back((3 * j) + 1);
        vertexUses[thirdVertex].push_back((3 * j) +2);
        
        //calculate mean z value of face, will be used for sorting faces
        meanZValues.push_back((inputVertices[firstVertex].z + inputVertices[secondVertex].z + inputVertices[thirdVertex].z) / 3.0);
        
        //checking max edge length
        if(glm::length(inputVertices[firstVertex] - inputVertices[secondVertex]) > max){
            max = glm::length(inputVertices[firstVertex] - inputVertices[secondVertex]);
        }
        if(glm::length(inputVertices[firstVertex] - inputVertices[thirdVertex]) > max){
            max = glm::length(inputVertices[firstVertex] - inputVertices[thirdVertex]);
        }
        if(glm::length(inputVertices[secondVertex] - inputVertices[thirdVertex]) > max){
            max = glm::length(inputVertices[secondVertex] - inputVertices[thirdVertex]);
        }
        
        //calculate face normal, at this time equal for all 3 vertices
        glm::vec3 firstVector = inputVertices[secondVertex] - inputVertices[firstVertex];
        glm::vec3 secondVector = inputVertices[thirdVertex] - inputVertices[firstVertex];
        glm::vec3 normal = glm::cross(firstVector, secondVector);
        
        //add vertices and normal of each face to temporary arrays
        tempVertices.push_back(inputVertices[firstVertex]);
        tempNormals.push_back(normal);
        
        tempVertices.push_back(inputVertices[secondVertex]);
        tempNormals.push_back(normal);
        
        tempVertices.push_back(inputVertices[thirdVertex]);
        tempNormals.push_back(normal);
        
    }
    //normal smoothing
    for (int k = 0 ; k < vertexNumber; k++){
        glm::vec3 newNormal;
        for (int l = 0 ; l < vertexUses[k].size(); l++){
            newNormal += tempNormals[vertexUses[k][l]];
        }
        newNormal = glm::normalize(newNormal);
        for (int m = 0; m < vertexUses[k].size(); m++){
            tempNormals[vertexUses[k][m]] = newNormal;
        }
    }
    
    //sorting vertices by Z coordinate
    float min;
    int minIndex = -1;
    for( int n = 0; n < polygonNumber; n++){
        min = MAXFLOAT;
        for(int o = 0; o < meanZValues.size(); o++){
            if(meanZValues[o] < min){
                min = meanZValues[o];
                minIndex = o;
            }
        }
        outVerticesZ.push_back(tempVertices[3 * minIndex]);
        outNormalsZ.push_back(tempNormals[3 * minIndex]);
        outVerticesZ.push_back(tempVertices[(3 * minIndex) + 1]);
        outNormalsZ.push_back(tempNormals[(3 * minIndex) + 1]);
        outVerticesZ.push_back(tempVertices[(3 * minIndex) + 2]);
        outNormalsZ.push_back(tempNormals[(3 * minIndex) + 2]);
        meanZValues[minIndex] = MAXFLOAT;
    }
    maxSide = max;
    return;
}

//simple version of the object loader for OFF files, only imports and does normal smoothing
void loadOFF(
             const char * path,
             std::vector<glm::vec3> & outVertices,
             std::vector<glm::vec3> & outNormals
             ){
    printf("Loading OFF file %s...\n", path);
    /* create temporary arrays and variables*/
    int vertexNumber = 0;
    int polygonNumber = 0;
    int lineNumber = 0;
    std::vector<float> meanZValues;
    std::vector<float> meanYValues;
    std::vector<float> meanXValues;
    std::vector<glm::vec3> inputVertices;
    std::vector<glm::vec3> tempVertices;
    std::vector<glm::vec3> tempNormals;
    //open file
    FILE * file = fopen(path, "r");
    //file not found
    if( file == NULL ){
        printf("Impossible to open the file ");
        getchar();
        return;
    }
    
    char fileHeader[128];
    int res = fscanf(file, "%s", fileHeader);
    if (res == EOF){
        return;
        //read first line, if it is "OFF" read second line and store number of vertices and faces
    }else if(strcmp(fileHeader, "OFF\n")){
        fscanf(file, "%d %d %d\n", &vertexNumber, &polygonNumber, &lineNumber);
        //        printf( "%d %d %d\n", vertex_number, polygon_number, line_number);
    }
    std::vector<std::vector<int>> vertexUses (vertexNumber);
    //read all vertices and store them in input_vertices
    for(int i = 0; i < vertexNumber ; i++){
        glm::vec3 vertex;
        fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
        inputVertices.push_back(vertex);
    }
    //read all faces composition and put the 3 vertices in the temp_array, also mark vertices usage in vertex_uses for use in normal smoothing
    for(int j = 0; j < polygonNumber ; j++){
        int vertexCount, firstVertex, secondVertex, thirdVertex;
        fscanf(file, "%d %d %d %d\n", &vertexCount, &firstVertex, &secondVertex, &thirdVertex);
        
        //mark vertices use
        vertexUses[firstVertex].push_back(3 * j);
        vertexUses[secondVertex].push_back((3 * j) + 1);
        vertexUses[thirdVertex].push_back((3 * j) +2);
        
        
        //calculate face normal, at this time equal for all 3 vertices
        glm::vec3 firstVector = inputVertices[secondVertex] - inputVertices[firstVertex];
        glm::vec3 secondVector = inputVertices[thirdVertex] - inputVertices[firstVertex];
        glm::vec3 normal = glm::cross(firstVector, secondVector);
        
        //add vertices and normal of each face to temporary arrays
        tempVertices.push_back(inputVertices[firstVertex]);
        tempNormals.push_back(normal);
        
        tempVertices.push_back(inputVertices[secondVertex]);
        tempNormals.push_back(normal);
        
        tempVertices.push_back(inputVertices[thirdVertex]);
        tempNormals.push_back(normal);
        
    }
    //normal smoothing
    for (int k = 0 ; k < vertexNumber; k++){
        glm::vec3 newNormal;
        for (int l = 0 ; l < vertexUses[k].size(); l++){
            newNormal += tempNormals[vertexUses[k][l]];
        }
        newNormal = glm::normalize(newNormal);
        for (int m = 0; m < vertexUses[k].size(); m++){
            tempNormals[vertexUses[k][m]] = newNormal;
        }
    }
    outNormals = tempNormals;
    outVertices = tempVertices;
    
    //    printf("and we are done \n");
    return;
}
/*utility function to do some modifications the mesh, the new list of points is printed in the stdout */
void convertOFF(const char * path){
    printf("converting OFF file %s...\n", path);
    /* create temporary arrays and variables*/
    int vertexNumber = 0;
    int polygonNumber = 0;
    int lineNumber = 0;
    
    FILE * file = fopen(path, "r");
    //file not found
    if( file == NULL ){
        printf("Impossible to open the file ");
        getchar();
        return;
    }
    
    char fileHeader[128];
    int res = fscanf(file, "%s", fileHeader);
    if (res == EOF){
        return;
        //read first line, if it is "OFF" read second line and store number of vertices and faces
    }else if(strcmp(fileHeader, "OFF\n")){
        fscanf(file, "%d %d %d\n", &vertexNumber, &polygonNumber, &lineNumber);
        //        printf( "%d %d %d\n", vertex_number, polygon_number, line_number);
    }
    //read all vertices and store them in input_vertices
    for(int i = 0; i < vertexNumber ; i++){
        glm::vec3 vertex;
        fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
        glm::mat4 TransMatrix = glm::translate(glm::mat4 (1.0f), glm::vec3(0.0, 0.0, 0.0));
        float angle = 0.0;
        glm::mat4 RotMatrix = glm::rotate(TransMatrix, angle, glm::vec3(0.0,1.0,0.0));
        glm::mat4 FinalMatrix = glm::scale(RotMatrix, glm::vec3(2.0, 2.0, 2.0));
        vertex = glm::vec3( FinalMatrix * glm::vec4(vertex, 1.0));
        printf("%f %f %f \n",vertex.x, vertex.y, vertex.z);
    }
    //read all faces composition and put the 3 vertices in the temp_array, also mark vertices usage in vertex_uses for use in normal smoothing
    for(int j = 0; j < polygonNumber ; j++){
        int vertexCount, firstVertex, secondVertex, thirdVertex;
        fscanf(file, "%d %d %d %d\n", &vertexCount, &firstVertex, &secondVertex, &thirdVertex);
        //printf("%d %d %d %d\n", vertexCount, firstVertex, secondVertex, thirdVertex);
        
        
        
    }
    return;
}


