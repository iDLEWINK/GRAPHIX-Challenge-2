/*The order matters. glad should be above GLFW*/
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// For 3D
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

// For string
#include <string>
#include <iostream>

// GLM Headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Camera Class
#include "PerspectiveCamera.h"
#include "OrthoCamera.h"

// Light Class
#include "DirectionalLight.h"
#include "PointLight.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    float screenWidth = 1280.f;
    float screenHeight = 720.f;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(screenWidth, screenHeight, "Josh Aaron Khyle S. Uson", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();
    

    /* Enable Depth Test to fix layer ordering of rendering */
    glEnable(GL_DEPTH_TEST);
    /* Set Viewport */
    glViewport(0, 0, screenWidth, screenHeight);




    /* TEXTURES HERE */
    stbi_set_flip_vertically_on_load(true); // For image flip; Loads it in an upright manner.    

    GLuint texture; // Create list of textures as variable
    const char* texture_path = "3D/shroom.png";

    int img_width, img_height, color_channels;
    /* Loaded texture */
    unsigned char* tex_bytes = stbi_load(texture_path, // Texture path
        &img_width, // Fill width
        &img_height, // Fill height
        &color_channels, // Number of color channels
        0);

    glGenTextures(1, &texture); // Generate 1 texture and assign it to texture variable

    // Can have up to GL_TEXTURE60. You process succeeding textures individually if you have more than 1 on you wish to apply on an object
    glActiveTexture(GL_TEXTURE0);

    // Tell OpenGL we're modifying texture at index 0
    glBindTexture(GL_TEXTURE_2D, texture); // Bind it for now

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA, //GL_RGB = jpegs or pngs w/o alphas; GL_RGBA = pngs or images w/ alpha; some pngs does not have an alpha channel
        img_width,
        img_height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE, // Type of our loaded image
        tex_bytes // loaded texture in bytes
    );

    /* Assign the loaded texture; Generate the mipmaps to the current texture */
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(tex_bytes); // Free from memory   
    /* TEXTURE SETUP END */










    /* SHADER PROGRAM CREATION */
    //All of these are conversions for vert
    std::fstream vertSrc("Shaders/sample.vert");
    std::stringstream vertBuff;
    vertBuff << vertSrc.rdbuf();
    std::string vertString = vertBuff.str();
    const char* v = vertString.c_str();

    //All of these are conversions for frag
    std::fstream fragSrc("Shaders/sample.frag");
    std::stringstream fragBuff;
    fragBuff << fragSrc.rdbuf();
    std::string fragString = fragBuff.str();
    const char* f = fragString.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &v, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &f, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);
    // Use the shaderProgram
    glUseProgram(shaderProgram);


    /* OBJECT LOADING + SETUP MESH START */
    /*Initialized list of VAOs and VBOs has an ID of unsigned integer - will be called by glGenVertexArrays*/
    std::string paths[2] = { "3D/shroom.obj", "3D/myCube.obj"};
    std::vector<GLfloat> fullVertexData;
    
    // Initialize VAOs, VBOs, and EBO
    /*
    VAO0 - spoon
    VAO1 - djSword (NO TEXCOORDS)
    VAO2 - teacup (NO NORMALS)    
    */

    GLuint VAO[2], VBO[2], EBO;
    // Generate and assign ID to each VAO
    glGenVertexArrays(2, VAO);
    // Generate and assign ID to each VBO
    glGenBuffers(2, VBO);
    // Generate and assign ID to EBO
    glGenBuffers(1, &EBO);

    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warning, error;
    tinyobj::attrib_t attributes;

    bool success;

    GLintptr normPtr;
    GLintptr uvPtr;




    /* VAO0 - Mushroom COMPLETE */
    success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &materials,
        &warning,
        &error,
        paths[0].c_str()
    );
        
    for (int j = 0; j < shapes[0].mesh.indices.size(); j++) {
        tinyobj::index_t vData = shapes[0].mesh.indices[j];
        /* XYZ */
        int vertexIndex = vData.vertex_index * 3; // multiplied by 3 cause of x, y, z and get the base offset of vertex itself
        /* NX NY NZ */
        int normalIndex = vData.normal_index * 3; // multiplied by 3 cause of nx, ny, nz
        /* UV */
        int uvIndex = vData.texcoord_index * 2; // multiplied by 2 cause of u and v

        /* POSITION VERTICES */
        fullVertexData.push_back(attributes.vertices[vertexIndex]); // X        
        fullVertexData.push_back(attributes.vertices[vertexIndex + 1]); // Y        
        fullVertexData.push_back(attributes.vertices[vertexIndex + 2]); // Z

        /* NORMALS */
        fullVertexData.push_back(attributes.normals[normalIndex]); // NX
        fullVertexData.push_back(attributes.normals[normalIndex + 1]); // NY
        fullVertexData.push_back(attributes.normals[normalIndex + 2]); // NZ

        /* TEXTURE COORDS */
        fullVertexData.push_back(attributes.texcoords[uvIndex]); // U        
        fullVertexData.push_back(attributes.texcoords[uvIndex + 1]); // V
    }       

    // Assigns or binds VAO - any calls after this will automatically point to VAO; Tells OpenGL we're working with that specified VAO
    glBindVertexArray(VAO[0]);
    // Converts VBO to an array buffer that stores our vertex position; Tells OpenGL we're working with this VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    // Assigns data to VBO
    glBufferData(
        GL_ARRAY_BUFFER, // What data is in the buffer
        sizeof(GL_FLOAT) * fullVertexData.size(),
        fullVertexData.data(),
        GL_STATIC_DRAW //The position won't update therefore it's static
    );

    /* Tell OpenGL how to interpret the array */
    /* VERTEX AttribPointer - For vertex offset */
    glVertexAttribPointer(
        0, // Position - SPECIAL INDEX 0, 1, and 2
        3, // X, Y, Z ( this just points to the position so we can retain 3 even with U and V )
        GL_FLOAT, // Type of ARRAY
        GL_FALSE, // Should we normalize this?
        8 * sizeof(GL_FLOAT), // Changed from 3 to 8 since we now have X Y Z NX NY NZ U V
        (void*)0
    );

    /* NORMALS AttribPointer - For lighting */
    normPtr = 3 * sizeof(GLfloat); // X Y Z offset in order to reach NX NY NZ in the memory
    glVertexAttribPointer(
        1, // NORMAL is assigned to index 1
        3, // NX, NY, NZ
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(GL_FLOAT),
        (void*)normPtr
    );

    /* UV AttribPointer - For texture */
    uvPtr = 6 * sizeof(GLfloat); // X Y Z NX NY NZ offset in order to reach U V in the memory
    glVertexAttribPointer(
        2, // UV assigned to index 2
        2,  // U and V
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(GL_FLOAT),
        (void*)uvPtr
    );

    // SPECIAL INDICES
    // 0 is for the Position of the vertex
    // 1 is for the Normals
    // 2 is for the Textures
    glEnableVertexAttribArray(0); // Position
    glEnableVertexAttribArray(1); // Normals
    glEnableVertexAttribArray(2); // Textures    







    /* VAO1 - UNLIT POINT LIGHT OBJECT */
    /* Load the mesh */
    success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &materials,
        &warning,
        &error,
        paths[1].c_str()
    );

    /* Setup indices for EBO */
    std::vector<GLuint> mesh_indices;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        mesh_indices.push_back(shapes[0].mesh.indices[i].vertex_index);
    }

    // Assigns or binds VAO - any calls after this will automatically point to VAO; Tells OpenGL we're working with that specified VAO
    glBindVertexArray(VAO[1]);

    // Converts VBO to an array buffer that stores our vertex position; Tells OpenGL we're working with this VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    // Assigns data to VBO
    glBufferData(
        GL_ARRAY_BUFFER, // What data is in the buffer
        sizeof(GL_FLOAT) * attributes.vertices.size(),
        &attributes.vertices[0],
        GL_STATIC_DRAW //The position won't update therefore it's static
    );

    /* Tell OpenGL how to interpret the array */
    /* VERTEX AttribPointer - For vertex offset */
    glVertexAttribPointer(
        0, // Position - SPECIAL INDEX 0, 1, and 2
        3, // X, Y, Z
        GL_FLOAT, // Type of ARRAY
        GL_FALSE, // Should we normalize this?
        3 * sizeof(GL_FLOAT), // Changed from 3 to 6 since we now have X Y Z NX NY NZ
        (void*)0
    );

    /* Bind to EBO */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * mesh_indices.size(),
        mesh_indices.data(),
        GL_STATIC_DRAW);

    // SPECIAL INDICES
    // 0 is for the Position of the vertex
    // 1 is for the Normals
    // 2 is for the Textures
    glEnableVertexAttribArray(0); // Position
   
    // We're done modifying - bind it to 0
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // We're done modifying - bind it to 0
    glBindVertexArray(0);
    /* OBJECT LOADING + SETUP MESH END */    






    /* PERSPECTIVE CAMERA */
    PerspectiveCamera perspectiveCamera(
        glm::vec3(0.0f, 0.0f, 10.0f),   // Camera Position
        glm::vec3(0.0f, 0.0f, 0.0f),    // Camera Center
        glm::vec3(0.0f, 1.0f, 0.0f),    // World Up
        90.0f,                          // Yaw
        0.0f,                           // Pitch
        0.1f,                           // Sensitivity
        screenWidth,                    // Screen Width
        screenHeight                    // Screen Height
    );

    perspectiveCamera.setProjectionMatrix(
        45.0f,                      // FOV in degrees        
        0.1f,                       // Z Near
        100.0f,                     // Z Far
        screenWidth / screenHeight  // Aspect Ratio
    );   













    /* LIGHTING */

    PointLight pointLight(
        glm::vec3(10, 10, 0),   // Light Position - Position of light origin (X, Y, Z)
        glm::vec3(1, 1, 1),     // Light Color - RGB lighting of light source
        glm::vec3(1, 1, 1),     // Ambient Color - RGB lighting of reflected or ambient light
        0.4f,                   // Ambient Strength - Intensity of reflected or ambient light
        glm::vec3(1, 1, 1),     // Specular Color - RGB lighting of specular light
        1.0f,                   // Specular Strength - intensity of specular light
        16.0f,                  // Specular Phong - concentration of specular light
        1.0f,                   // Constant Value for Attenuation
        0.0014f,                // Linear Value for Attenuation
        0.000007f               // Quadratic Value for Attenuation
        );














    /* TRANSFORMATION MATRIX */
    float z = 3.0f;
    /* Create identity matrix */
    glm::mat4 identity_matrix4 = glm::mat4(1.0f);

    /* Base transformation matrix */
    glm::mat4 model_matrix = identity_matrix4;
    glm::mat4 light_obj_matrix = identity_matrix4;

    /* POSITIONS */
    model_matrix = glm::translate(model_matrix, glm::vec3(0.0f, 0.0f, z));
    light_obj_matrix = glm::translate(light_obj_matrix, glm::vec3(3.0f, 2.0f, z));

    /* SCALING */
    // MODEL OBJECT
    float l_scale_x, l_scale_y, l_scale_z; 
    l_scale_x = l_scale_y = l_scale_z = 0.005f;
    model_matrix = glm::scale(model_matrix, glm::vec3(l_scale_x, l_scale_y, l_scale_z));
    // LIGHT OBJECT
    float c_scale_x, c_scale_y, c_scale_z;
    c_scale_x = c_scale_y = c_scale_z = 0.25f;
    light_obj_matrix = glm::scale(light_obj_matrix, glm::vec3(c_scale_x, c_scale_y, c_scale_z));

    float theta, rot_x, rot_y, rot_z;
    /* ROTATION Y - AXIS */
    rot_x = rot_z = 0.0f;
    rot_y = 1.0f;
    // MODEL OBJECT
    theta = 0.0f;
    model_matrix = glm::rotate(model_matrix, glm::radians(theta), glm::normalize(glm::vec3(rot_x, rot_y, rot_z)));
    // LIGHT OBJECT
    theta = 0.0f;
    light_obj_matrix = glm::rotate(light_obj_matrix, glm::radians(theta), glm::normalize(glm::vec3(rot_x, rot_y, rot_z)));

    /* ROTATION X - AXIS */
    rot_y = rot_z = 0.0f;
    rot_x = 1.0f;
    // MODEL OBJECT
    theta = 0.0f;
    model_matrix = glm::rotate(model_matrix, glm::radians(theta), glm::normalize(glm::vec3(rot_x, rot_y, rot_z)));
    // LIGHT OBJECT
    theta = 0.0f;
    light_obj_matrix = glm::rotate(light_obj_matrix, glm::radians(theta), glm::normalize(glm::vec3(rot_x, rot_y, rot_z)));
    

    glm::mat4 projection_matrix, view_matrix;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);              

        projection_matrix = perspectiveCamera.getProjectionMatrix();
        view_matrix = perspectiveCamera.getViewMatrix();

        /* VARIABLES FOR CAMERA */
        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection_matrix));
        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view_matrix));

        /* VARIABLES FOR OBJECT POSITION, ROTATION, SCALE - (TO BE USED LATER) */
        unsigned int transformationLoc = glGetUniformLocation(shaderProgram, "transform"); // transform is the variable from sample.vert

        /* VARIABLES FOR TEXTURE*/
        GLuint tex0Address = glGetUniformLocation(shaderProgram, "tex0"); // Get the address
        glUniform1i(tex0Address, 0); // Comes from GL_TEXTURE 0

        /* VARIABLES FOR LIGHTING */
        // LIGHT POSITION AND COLOR
        unsigned int lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
        glUniform3fv(lightPosLoc, 1, glm::value_ptr(pointLight.lightPos));
        unsigned int lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
        glUniform3fv(lightColorLoc, 1, glm::value_ptr(pointLight.lightColor));

        // AMBIENT STRENGTH AND COLOR
        unsigned int ambientStrLoc = glGetUniformLocation(shaderProgram, "ambientStr");
        glUniform1f(ambientStrLoc, pointLight.ambientStr);
        unsigned int ambientColorLoc = glGetUniformLocation(shaderProgram, "ambientColor");
        glUniform3fv(ambientColorLoc, 1, glm::value_ptr(pointLight.ambientColor));

        // CAMERA POSITION, SPECULAR STRENGTH, AND SPECULAR PHONG
        unsigned int cameraPosLoc = glGetUniformLocation(shaderProgram, "cameraPos");
        glUniform3fv(cameraPosLoc, 1, glm::value_ptr(perspectiveCamera.getCameraPos()));
        unsigned int specStrLoc = glGetUniformLocation(shaderProgram, "specStr");
        glUniform1f(specStrLoc, pointLight.specStr);
        unsigned int specPhongLoc = glGetUniformLocation(shaderProgram, "specPhong");
        glUniform1f(specPhongLoc, pointLight.specPhong);
        

        /* MODEL OBJECT ITEM - VAO0 and TEXTURE0 */
        // DRAW THE TEXTURE0
        glBindTexture(GL_TEXTURE_2D, texture); // Call OpenGL we're using that texture
        // MODEL OBJECT TRANSFORM MATRIX
        glUniformMatrix4fv(transformationLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));
        // DRAW OBJECT0 
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / 8); // divided by 8 to get the number of vertices to draw




        /* LIGHT OBJECT ITEM - VAO1 */       
        // LIGHT OBJECT TRANSFORM MATRIX
        glUniformMatrix4fv(transformationLoc, 1, GL_FALSE, glm::value_ptr(light_obj_matrix));
        // DRAW OBJECT1 
        glBindVertexArray(VAO[1]);
        glDrawElements(GL_TRIANGLES,
            mesh_indices.size(),
            GL_UNSIGNED_INT,
            0);

     
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    // Clean up
    glDeleteVertexArrays(3, VAO);
    glDeleteBuffers(3, VBO);
       
    glfwTerminate();
    return 0;
}