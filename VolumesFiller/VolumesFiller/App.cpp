//
//  App.cpp
//  VolumesFiller
//
//  Created by Matteo Morisoli on 06/06/15.
//  Copyright (c) 2015 Matteo Morisoli. All rights reserved.
//

#include "App.h"
static bool showmodel = true;
static int viewDistance = 4;

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void changeShowModel(){
    if(showmodel){
        showmodel = false;
    }else{
        showmodel = true;
    }
}

static void modifyViewDistance(bool x){
    if(x){
        if(viewDistance < 8){
            viewDistance++;
        }
    }else{
        if(viewDistance > 1){
            viewDistance--;
        }
    }
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS){
        changeShowModel();
    }
    if (key == GLFW_KEY_UP && action == GLFW_PRESS){
        modifyViewDistance(false);
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS){
        modifyViewDistance(true);
    }
}

void mainLoop(double spheresRadius, double gridSize, std::string lattice, std::string mesh){
    // Window setup and key and error binding
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    window = glfwCreateWindow(1024, 1024, "Volumes", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetKeyCallback(window, key_callback);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(1.0, 1.0, 1.0, 0.0);
    
    clock_t startTime;
    startTime = clock();
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    // Loading shader files
    GLuint programID = LoadShaders( "/Users/Metius/Desktop/VolumesFiller/Shaders/InstanceShading.vertexshader", "/Users/Metius/Desktop/VolumesFiller/Shaders/StandardShading.fragmentshader" );
    
    //loading object and creating vectors of informations
    std::vector<glm::vec3> verticesZ;
    std::vector<glm::vec3> normalsZ;
    double maxObject;
    if(mesh == "Armadillo"){
        loadOFFComplex("/Users/Metius/Desktop/VolumesFiller/OFF_Models/armadillo.off", verticesZ, normalsZ,  maxObject);
    }else if(mesh == "Genus3"){
        loadOFFComplex("/Users/Metius/Desktop/VolumesFiller/OFF_Models/genus3.off", verticesZ, normalsZ,  maxObject);
    }else if(mesh == "Eight"){
        loadOFFComplex("/Users/Metius/Desktop/VolumesFiller/OFF_Models/eight.off", verticesZ, normalsZ,  maxObject);
    }else if(mesh == "Cube"){
        loadOFFComplex("/Users/Metius/Desktop/VolumesFiller/OFF_Models/cube.off", verticesZ, normalsZ,  maxObject);
    }else{
        loadOFFComplex("/Users/Metius/Desktop/VolumesFiller/OFF_Models/hiressphere.off", verticesZ, normalsZ,  maxObject);
    }
    
//    double spheresRadius = 0.064;
//    double gridSize = 3.0;
    
    //convertOFF("/Users/Metius/Desktop/Bachelor\ project/Volumes/Volumes/Volumes/genus3.off");
    std::vector<glm::vec3> collisionVertices = verticesZ;
    std::vector<glm::vec3> indexedVerticesZ;
    std::vector<glm::vec3> indexedNormalsZ;
    std::vector<glm::vec3> reversedVerticesZ = verticesZ;
    std::vector<glm::vec3> reversedNormalsZ = normalsZ;
    std::reverse(reversedVerticesZ.begin(), reversedVerticesZ.end());
    std::reverse(reversedNormalsZ.begin(), reversedNormalsZ.end());
    std::vector<glm::vec3> indexedReversedVerticesZ;
    std::vector<glm::vec3> indexedReversedNormalsZ;
    std::vector<unsigned short> VBOIndicesZ;
    indexVBO(verticesZ, normalsZ, VBOIndicesZ, indexedVerticesZ, indexedNormalsZ);
    std::vector<unsigned short> VBOreversedZ;
    indexVBO(reversedVerticesZ, reversedNormalsZ, VBOreversedZ, indexedReversedVerticesZ, indexedReversedNormalsZ);
    
    //Loading second object
    std::vector<glm::vec3> sphere_vertices;
    std::vector<glm::vec3> sphere_normals;
    printf("max edge length: %f \n", maxObject);
    double divisor;
    //loading different meshes depending on sphere size
    if(spheresRadius > 0.1){
        loadOFF("/Users/Metius/Desktop/VolumesFiller/OFF_Models/hiressphere.off", sphere_vertices, sphere_normals);
        divisor = 5.0;
    }else if (spheresRadius <= 0.1 && spheresRadius > 0.01){
        loadOFF("/Users/Metius/Desktop/VolumesFiller/OFF_Models/medressphere.off", sphere_vertices, sphere_normals);
        divisor = 5.0;
    }else{
        loadOFF("/Users/Metius/Desktop/VolumesFiller/OFF_Models/lowressphere.off", sphere_vertices, sphere_normals);
        divisor = 2.0;
        if(spheresRadius <= 0.002){
            divisor = 1.0;
        }
    }
    int gridDivisor = (int) (gridSize / (divisor * maxObject)) - 1;
    if(gridDivisor < 1){
        gridDivisor = 1;
    }
    printf("grid divisor: %d \n", gridDivisor);
    std::vector<glm::vec3> sphere_indexed_vertices;
    std::vector<glm::vec3> sphere_indexed_normals;
    std::vector<unsigned short> sphere_VBO_indices;
    indexVBO(sphere_vertices, sphere_normals, sphere_VBO_indices, sphere_indexed_vertices, sphere_indexed_normals);
    
    //object buffers
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, indexedVerticesZ.size() * sizeof(glm::vec3), &indexedVerticesZ[0], GL_STATIC_DRAW);
    
    GLuint normalBuffer;
    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, indexedNormalsZ.size() * sizeof(glm::vec3), &indexedNormalsZ[0], GL_STATIC_DRAW);
    
    GLuint reversedVertexBuffer;
    glGenBuffers(1, &reversedVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, reversedVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, indexedReversedVerticesZ.size() * sizeof(glm::vec3), &indexedReversedVerticesZ[0], GL_STATIC_DRAW);
    
    GLuint reversedNormalBuffer;
    glGenBuffers(1, &reversedNormalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, reversedNormalBuffer);
    glBufferData(GL_ARRAY_BUFFER, indexedReversedNormalsZ.size() * sizeof(glm::vec3), &indexedReversedNormalsZ[0], GL_STATIC_DRAW);
    
    // Generate a buffer for the indices as well
    GLuint elementBuffer;
    glGenBuffers(1, &elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, VBOIndicesZ.size() * sizeof(unsigned short), &VBOIndicesZ[0], GL_STATIC_DRAW);
    
    GLuint reversedElementBuffer;
    glGenBuffers(1, &reversedElementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, reversedElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, VBOreversedZ.size() * sizeof(unsigned short), &VBOreversedZ[0], GL_STATIC_DRAW);
    
    GLuint sphereElementBuffer;
    glGenBuffers(1, &sphereElementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere_VBO_indices.size() * sizeof(unsigned short), &sphere_VBO_indices[0], GL_STATIC_DRAW);
    
    //sphere buffers
    GLuint sphereVertexBuffer;
    glGenBuffers(1, &sphereVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, sphereVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sphere_indexed_vertices.size() * sizeof(glm::vec3), &sphere_indexed_vertices[0], GL_STATIC_DRAW);
    
    GLuint sphereNormalBuffer;
    glGenBuffers(1, &sphereNormalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, sphereNormalBuffer);
    glBufferData(GL_ARRAY_BUFFER, sphere_indexed_normals.size() * sizeof(glm::vec3), &sphere_indexed_normals[0], GL_STATIC_DRAW);
    
    // Projection matrix : 45∞ Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 4.0f, 0.1f, 100.0f);
    // Camera matrix
    glm::mat4 View       = glm::lookAt(
                                       glm::vec3(0,0,4), // Camera is at (4,3,3), in World Space
                                       glm::vec3(0,0,0), // and looks at the origin
                                       glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                                       );
    
    glm::mat4  i = glm::mat4(1.0f);
    glm::mat4 Model = i;
    
    //defining coefficents for shaders
    glm::vec3 objectDiffuse = glm::vec3(0.2,0.9,0.2);
    glm::vec3 objectAmbient = glm::vec3(0.2,0.5,0.2);
    glm::vec3 objectSpecular = glm::vec3(0.3,0.3,0.3);
    GLdouble objectShiny = 30.0;
    
    glm::vec3 sphereDiffuse = glm::vec3(0.9,0.2,0.2);
    glm::vec3 sphereAmbient = glm::vec3(0.5,0.3,0.3);
    glm::vec3 sphereSpecular = glm::vec3(0.3,0.3,0.3);
    GLdouble sphereShiny = 30.0;
    
    glm::vec3 ambientLight = glm::vec3(0.6,0.6,0.6);
    glm::vec3 lightColor = glm::vec3(0.4,0.4,0.4);
    GLdouble lightIntensity = 2.0;
    
    //Loading coefficents for shaders
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
    GLuint ModelMatrixID = glGetUniformLocation(programID, "M");
    //GLuint vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
    //GLuint vertexNormal_modelspaceID = glGetAttribLocation(programID, "vertexNormal_modelspace");
    GLuint DiffuseMaterialID = glGetUniformLocation(programID, "DiffuseMaterial");
    GLuint AmbientMaterialID = glGetUniformLocation(programID, "AmbientMaterial");
    GLuint SpecularMaterialID = glGetUniformLocation(programID, "SpecularMaterial");
    GLuint ShinynessID = glGetUniformLocation(programID, "ShinynessMaterial");
    GLuint AmbientLightID = glGetUniformLocation(programID, "AmbientLight");
    GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
    GLuint LightColorID = glGetUniformLocation(programID, "LightColor");
    GLuint LightIntensityID = glGetUniformLocation(programID, "LightIntensity");
    //GLuint SpheresPositionID = glGetUniformLocation(programID, "SpheresPosition");
    GLuint SpheresRadiusID = glGetUniformLocation(programID, "SphereRadius");
    
    //main simulation step
    int nodesLength = gridDivisor * gridDivisor *gridDivisor;
    std::vector<std::vector<std::vector<glm::vec3>>> allPoints;
    if(lattice == "Hexagonal lattice"){
        allPoints = generateSpherePosABA(gridSize, spheresRadius);
    }else{
        allPoints = generateSpherePosABC(gridSize, spheresRadius);
    }
    std::thread * firstThreads = new std::thread[allPoints.size()];
    std::vector<std::vector<glm::vec3>> returnVectors(allPoints.size());
    for(int a = 0; a < allPoints.size(); a++){
        firstThreads[a] = std::thread( checkOutsideSpheresLineThreaded, allPoints[a], collisionVertices, maxObject, std::ref(returnVectors[a]));
    }
    for(int b = 0; b < allPoints.size(); b++){
        firstThreads[b].join();
    }
    
    std::vector<glm::vec3> insidePoints;
    //    insidePoints = checkOutsideSpheresLine(allPoints, collisionVertices, maxObject);
    for(int c = 0; c < allPoints.size(); c++){
        insidePoints.insert(insidePoints.end(), returnVectors[c].begin(), returnVectors[c].end());
    }
    
    Node * nodes = createNodeGrid(collisionVertices, insidePoints, gridSize, gridDivisor, spheresRadius);
    std::thread * threads = new std::thread[nodesLength];
    for(int a = 0; a < nodesLength; a++){
        threads[a] = std::thread( checkIntersectionPartitioned, std::ref(nodes[a]), spheresRadius, maxObject);
    }
    for(int b = 0; b < nodesLength; b++){
        threads[b].join();
    }
    std::set<int> intersectedSpheresIndices = mergeEliminatedSpheres(nodes, nodesLength);
    std::vector<glm::vec3> points = removeIntersectedSpheres(intersectedSpheresIndices, insidePoints);
    std::vector<glm::vec3> moddedPoints;
    
    for(int z = 0; z < points.size(); z++){
        moddedPoints.push_back(points[z]);
    }
    
    GLuint spheresPositionBuffer;
    glGenBuffers(1, &spheresPositionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, spheresPositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, moddedPoints.size() * sizeof(glm::vec3), &moddedPoints[0], GL_STATIC_DRAW);
    
    std::vector<glm::vec3> emptyPoints;
    emptyPoints.push_back(glm::vec3(0.0,0.0,0.0));
    
    GLuint PositionBuffer;
    glGenBuffers(1, &PositionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, PositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, emptyPoints.size() * sizeof(glm::vec3), &emptyPoints[0], GL_STATIC_DRAW);
    
    double objectVolume = calculateObjectVolume(collisionVertices);
    double spheresVolume = calculateSphereVolumes(spheresRadius, points.size());
    double percentage = calculateFillingPercentage(objectVolume, spheresVolume);
    double packingDensity = 74.048048;
    //measuring time
    clock_t endTime;
    endTime = clock();
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    
    //printing results
    printf("The number of triangles in the object is: %lu \n", collisionVertices.size()/3);
    printf("The number of spheres that form the approximation is: %lu \n", points.size());
    printf("The total volume of the spheres is: %f \n", spheresVolume);
    printf("The exact volume of the triangle mesh is: %f \n", objectVolume);
    printf("The filling percentage is: %f \n", percentage);
    printf("THe filling percentage compared to the packing density is: %f\n", percentage/packingDensity * 100.0);
    printf("Time needed to run simulation %f seconds\n", elapsed_seconds.count());
    
    //main loop
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwSetCursorPos(window, 0, 0);
    
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);
        
        //mouse stuff
        double xPos = 0, yPos = 0, horizontalAngle, verticalAngle;
        glfwGetCursorPos(window, &xPos, &yPos);
        horizontalAngle = xPos /80.0;
        verticalAngle = yPos /80.0;
        if( verticalAngle > 3.1/2.0){
            verticalAngle = 3.1/2.0;
            
        } else if(verticalAngle < -3.1/2.0){
            verticalAngle = -3.1/2.0;
        }
        glm::mat4 MovingView       = glm::lookAt(
                                                 glm::vec3(viewDistance * sin(horizontalAngle) * cos(verticalAngle),viewDistance * sin(verticalAngle),viewDistance * cos(horizontalAngle)* cos(verticalAngle)),
                                                 glm::vec3(0,0,0), // and looks at the origin
                                                 glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                                                 );
        
        glUniform3f(DiffuseMaterialID, sphereDiffuse.x, sphereDiffuse.y, sphereDiffuse.z);
        glUniform3f(AmbientMaterialID, sphereAmbient.x, sphereAmbient.y, sphereAmbient.z);
        glUniform3f(SpecularMaterialID, sphereSpecular.x, sphereSpecular.y, sphereSpecular.z);
        glUniform1f(ShinynessID, sphereShiny);
        glUniform1f(SpheresRadiusID, spheresRadius);
        
        //light position
        glm::vec3 lightPos = glm::vec3(400 * sin(glfwGetTime()/2.0),4,400 * cos(glfwGetTime()/2.0));
        glUniform3f(AmbientLightID, ambientLight.x, ambientLight.y, ambientLight.z);
        glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(LightColorID, lightColor.x, lightColor.y, lightColor.z);
        glUniform1f(LightIntensityID, lightIntensity);
        
        glDisable(GL_BLEND);
        
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, sphereVertexBuffer);
        glVertexAttribPointer(
                              0,  // The attribute we want to configure
                              3,                            // size
                              GL_FLOAT,                     // type
                              GL_FALSE,                     // normalized?
                              0,                            // stride
                              (void*)0                      // array buffer offset
                              );
        
        
        // 2rd attribute buffer : normals
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, sphereNormalBuffer);
        glVertexAttribPointer(
                              1,    // The attribute we want to configure
                              3,                            // size
                              GL_FLOAT,                     // type
                              GL_FALSE,                     // normalized?
                              0,                            // stride
                              (void*)0                      // array buffer offset
                              );
        
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, spheresPositionBuffer);
        glVertexAttribPointer(
                              2,    // The attribute we want to configure
                              3,                            // size
                              GL_FLOAT,                     // type
                              GL_FALSE,                     // normalized?
                              0,                            // stride
                              (void*)0                      // array buffer offset
                              );
        
        
        glVertexAttribDivisor(0, 0);
        glVertexAttribDivisor(1, 0);
        glVertexAttribDivisor(2, 1);
        
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereElementBuffer);
        glm::mat4 modelSphere = glm::mat4(1.0);
        modelSphere = glm::scale(modelSphere, glm::vec3(spheresRadius, spheresRadius, spheresRadius));
        glm::mat4 MVP2        = Projection * MovingView * modelSphere;
        
        
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP2[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &modelSphere[0][0]);
        glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &MovingView[0][0]);
        glDrawElementsInstanced(
                                GL_TRIANGLES,      // mode
                                (int) sphere_vertices.size(),    // count
                                GL_UNSIGNED_SHORT, // type
                                (void*)0,
                                (int) moddedPoints.size()// element array buffer offset
                                );
        
        glUniform1f(SpheresRadiusID, 1);
        glm::mat4 ObjectModel = glm::mat4(1.0);
        
        glm::mat4 MVP        = Projection * MovingView * ObjectModel;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ObjectModel[0][0]);
        glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &MovingView[0][0]);
        
        glUniform3f(DiffuseMaterialID, objectDiffuse.x, objectDiffuse.y, objectDiffuse.z);
        glUniform3f(AmbientMaterialID, objectAmbient.x, objectAmbient.y, objectAmbient.z);
        glUniform3f(SpecularMaterialID, objectSpecular.x, objectSpecular.y, objectSpecular.z);
        glUniform1d(ShinynessID, objectShiny);
        
        if(showmodel){
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            
            glEnableVertexAttribArray(0);
            if((glm::abs(horizontalAngle) <= 3.14/2.0) || (glm::abs(horizontalAngle) > 3.14 * 3.0 / 4.0)){
                glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
            }else{
                glBindBuffer(GL_ARRAY_BUFFER, reversedVertexBuffer);
            }
            glVertexAttribPointer(
                                  0,  // The attribute we want to configure
                                  3,                            // size
                                  GL_FLOAT,                     // type
                                  GL_FALSE,                     // normalized?
                                  0,                            // stride
                                  (void*)0                      // array buffer offset
                                  );
            
            // 3rd attribute buffer : normals
            glEnableVertexAttribArray(1);
            if((glm::abs(horizontalAngle) <= 3.14/2.0) || (glm::abs(horizontalAngle) > 3.14 * 3.0 / 4.0)){
                glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
            }else{
                glBindBuffer(GL_ARRAY_BUFFER, reversedNormalBuffer);
            }
            glVertexAttribPointer(
                                  1,    // The attribute we want to configure
                                  3,                            // size
                                  GL_FLOAT,                     // type
                                  GL_FALSE,                     // normalized?
                                  0,                            // stride
                                  (void*)0                      // array buffer offset
                                  );
            
            glEnableVertexAttribArray(2);
            glBindBuffer(GL_ARRAY_BUFFER, PositionBuffer);
            glVertexAttribPointer(
                                  2,    // The attribute we want to configure
                                  3,                            // size
                                  GL_FLOAT,                     // type
                                  GL_FALSE,                     // normalized?
                                  0,                            // stride
                                  (void*)0                      // array buffer offset
                                  );
            
            
            glVertexAttribDivisor(0, 0);
            glVertexAttribDivisor(1, 0);
            glVertexAttribDivisor(2, 1);
            
            if((glm::abs(horizontalAngle) <= 3.14/2.0) || (glm::abs(horizontalAngle) > 3.14 * 3.0 / 4.0)){
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
            }else{
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, reversedElementBuffer);
            }
            glDrawElementsInstanced(
                                    GL_TRIANGLES,      // mode
                                    (int) verticesZ.size(),    // count
                                    GL_UNSIGNED_SHORT, // type
                                    (void*)0,
                                    1// element array buffer offset
                                    );
        }
        
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteBuffers(1, &sphereVertexBuffer);
    glDeleteBuffers(1, &sphereNormalBuffer);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &normalBuffer);
    glDeleteBuffers(1, &elementBuffer);
    glDeleteBuffers(1, &sphereElementBuffer);
    glDeleteProgram(programID);
    
    glfwDestroyWindow(window);
    glfwTerminate();
}