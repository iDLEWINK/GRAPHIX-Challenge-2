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

/* SCREEN SIZE */
float screenWidth = 750.f;
float screenHeight = 750.f;

/* LIGHT INTENSITY CONTROL */
float p_light_sens = 0.025f;
float d_light_sens = 0.025f;
float d_light_intensity = 1.0f; // DEFAULT
float p_light_intensity = 1.0f; // DEFAULT

/* LIGHT OBJECT ROTATION */
float light_rot_x = 0;
float light_rot_y = 0;
float light_rot_z = 0;

/* MODEL OBJECT ROTATION */
float model_rot_x = 0;
float model_rot_y = 0;
float model_rot_z = 0;

float obj_sens = 2.0f;

bool isLightControl = false;
glm::vec3 control_rgb = glm::vec3(1.0f, 1.0f, 1.0f);

/* CAMERA MOVEMENT */
int cameraType = 1;
/* Set mouse to center */
bool firstMouse = true;
/* Controls intensity */
float cam_sens = 0.8f;
float lastX = screenWidth / 2.0;
float lastY = screenHeight / 2.0;
/* DEFAULT SET TO 0 - CENTER */
float xoffset = 0;
float yoffset = 0;

/* SIGNAL FLAGS */
bool isOffsetChanged = false;
bool isDLightChanged = false;
bool isPLightChanged = false;

/* MOUSE FEEDBACK */
void Mouse_Callback(GLFWwindow* window, double xpos, double ypos) {
    /* Circumvent the abrupt initial jump by using a sentinel value in recording first input as last */
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    /* Offset from previous frame to new loaded frame */
    xoffset = (xpos - lastX) * cam_sens;
    yoffset = (lastY - ypos) * cam_sens;

    /* Store the last x,y mouse position */
    lastX = xpos;
    lastY = ypos;

    isOffsetChanged = true;
}

/* KEYS FEEDBACK */
void Key_Callback(GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods)
{
    /* LIGHT INTENSITY CONTROL */
    /* DIRECTIONAL LIGHT */
    if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        d_light_intensity = -d_light_sens;
        isDLightChanged = true;
    }
    if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        d_light_intensity = d_light_sens;
        isDLightChanged = true;
    }
    /* POINT LIGHT */
    if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        p_light_intensity = -p_light_sens; 
        isPLightChanged = true;
    }
    if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        p_light_intensity = p_light_sens;
        isPLightChanged = true;
    }    

    /* MODEL MOVEMENT KEYS */
    /* Y - AXIS ROTATION */
    if (key == GLFW_KEY_D && action == GLFW_REPEAT) {
        if(isLightControl) {
            light_rot_y -= obj_sens; // Rotate light object position to the (negative) direction
            if (light_rot_y == -360.0f)
                light_rot_y = 0;
        }
        else {
            model_rot_y -= obj_sens; // Rotate model object position to the (negative) direction
            if (model_rot_y == -360.0f)
                model_rot_y = 0;
        }
    }
    if (key == GLFW_KEY_A && action == GLFW_REPEAT) {
        if(isLightControl) {
            light_rot_y += obj_sens; // Rotate light object position to the (positive) direction
            if (light_rot_y == 360.0f)
                light_rot_y = 0;
        }
        else {
            model_rot_y += obj_sens; // Rotate model object position to the (positive) direction
            if (model_rot_y == 360.0f)
                model_rot_y = 0;
        }
    }

    /* X - AXIS ROTATION */
    if (key == GLFW_KEY_W && action == GLFW_REPEAT) {
        if(isLightControl) {
            light_rot_x -= obj_sens; // Rotate light object position to the (negative) direction
            if (light_rot_x == -360.0f)
                light_rot_x = 0;
        }
        else {
            model_rot_x -= obj_sens; // Rotate model object position to the (negative) direction
            if (model_rot_x == -360.0f)
                model_rot_x = 0;
        }
    }
    if (key == GLFW_KEY_S && action == GLFW_REPEAT) {
        if(isLightControl) {
            light_rot_x += obj_sens; // Rotate light object position to the (positive) direction
            if (light_rot_x == 360.0f)
                light_rot_x = 0;
        }
        else {
            model_rot_x += obj_sens; // Rotate model object position to the (positive) direction
            if (model_rot_x == 360.0f)
                model_rot_x = 0;
        }
    }

    /* Z - AXIS ROTATION */
    if (key == GLFW_KEY_E && action == GLFW_REPEAT) {
        if(isLightControl) {
            light_rot_z -= obj_sens; // Rotate light object position to the (negative) direction
            if (light_rot_z == -360.0f)
                light_rot_z = 0;
        }
        else {
            model_rot_z -= obj_sens; // Rotate model object position to the (negative) direction
            if (model_rot_z == -360.0f)
                model_rot_z = 0;
        }
    }
    if (key == GLFW_KEY_Q && action == GLFW_REPEAT) {
        if(isLightControl) {
            light_rot_z += obj_sens; // Rotate light object position to the (positive) direction
            if (light_rot_z == 360.0f)
                light_rot_z = 0;
        }
        else {
            model_rot_z += obj_sens; // Rotate model object position to the (positive) direction
            if (model_rot_z == 360.0f)
                model_rot_z = 0;
        }
    }

    /* SWITCH BETWEEN MODEL OBJECT AND LIGHT OBJECT */
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        /* Switches the flag for the state */
        isLightControl = !isLightControl;
        /* CHANGE COLOR BEHAVIOR */
        if (isLightControl)
            control_rgb = glm::vec3(0.47f, 1.0f, 0.37f);
        else 
            control_rgb = glm::vec3(1.0f, 1.0f, 1.0f);
    }

    /* SWITCH BETWEEN CAMERA TYPES */
    // PERSPECTIVE CAMERA
    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        cameraType = 1;
    }
    // ORTHOGRAPHIC CAMERA
    if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        cameraType = 2;
    }
    
    //std::cout << "x: " << light_rot_x << " y: " << light_rot_y << " z: " << light_rot_z << "\n";
    
    /* EXIT APPLICATION */
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}





int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
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
    glfwSetKeyCallback(window, Key_Callback);
    glfwSetCursorPosCallback(window, Mouse_Callback);

    /* Enable Depth Test to fix layer ordering of rendering */
    glEnable(GL_DEPTH_TEST);
    /* Set Viewport */
    glViewport(0, 0, screenWidth, screenHeight);

    /* TEXTURES SETUP START */
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


    /* Main Object + Point & Directional Light */
    /* Light Object + Unlit */
    /* SHADER PROGRAM CREATION START */
    GLuint shaderPrograms[2];
    const char* frag_paths[2] = { "Shaders/object.frag", "Shaders/unlit.frag" };
    const char* vert_paths[2] = { "Shaders/object.vert", "Shaders/unlit.vert" };

    for (int i = 0; i < 2; i++) {
        //All of these are conversions for vert
        std::fstream vertSrc(vert_paths[i]);
        std::stringstream vertBuff;
        vertBuff << vertSrc.rdbuf();
        std::string vertString = vertBuff.str();
        const char* v = vertString.c_str();

        //All of these are conversions for frag
        std::fstream fragSrc(frag_paths[i]);
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

        shaderPrograms[i] = shaderProgram;
        glLinkProgram(shaderPrograms[i]);
    }
    /* SHADER PROGRAM CREATION END */

    /* OBJECT LOADING + SETUP MESH START */
    /*Initialized list of VAOs and VBOs has an ID of unsigned integer - will be called by glGenVertexArrays*/
    std::string paths[2] = { "3D/shroom.obj", "3D/myCube.obj"};
    std::vector<GLfloat> fullVertexData;
    
    // Initialize VAOs, VBOs, and EBO
    /*
    VAO0 - Mushroom; Main 3D Model
    VAO1 - Square; Light Box
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





    /********** CAMERA **********/
    /* PERSPECTIVE CAMERA */
    PerspectiveCamera perspectiveCamera(
        glm::vec3(0.0f, 0.0f, 10.0f),   // Camera Position
        glm::vec3(0.0f, 0.0f, 0.0f),    // Camera Center
        glm::vec3(0.0f, 1.0f, 0.0f),    // World Up
        90.0f,                          // Yaw
        0.0f,                           // Pitch
        screenWidth,                    // Screen Width
        screenHeight                    // Screen Height
    );

    perspectiveCamera.setProjectionMatrix(
        45.0f,                      // FOV in degrees        
        0.1f,                       // Z Near
        100.0f,                     // Z Far
        screenWidth / screenHeight  // Aspect Ratio
    );   

    /* ORTHOGRAPHIC CAMERA */
    OrthoCamera orthoCamera(
        glm::vec3(0.0f, 10.0f, 0.0f),   // Camera Position
        glm::vec3(0.0f, 0.0f, 0.0f),    // Camera Center
        glm::vec3(0.0f, 0.0f, -1.0f),   // World Up
        90.0f,                          // Pitch
        0.0f                            // Yaw
    );

    orthoCamera.setProjectionMatrix(
        -15.0f, 15.0f,  // Xmin, Xmax
        -15.0f, 15.0f,  // Ymin, Ymax
        -15.0f, 15.0f   // Zmin, Zmax
    );


    /********** LIGHTING **********/
    /* POINT LIGHT */
    PointLight pointLight(
        glm::vec3(3.0f, 2.0f, 0.0f),       // Light Position - Position of light origin (X, Y, Z)
        glm::vec3(1.0f, 1.0f, 1.0f),     // Light Color - RGB lighting of light source
        1.0f,                       // Light Strength - intensity of diffuse light  
        glm::vec3(1.0f, 1.0f, 1.0f),     // Ambient Color - RGB lighting of reflected or ambient light
        0.4f,                       // Ambient Strength - Intensity of reflected or ambient light
        glm::vec3(1.0f, 1.0f, 1.0f),   // Specular Color - RGB lighting of specular light
        1.0f,                       // Specular Strength - intensity of specular light
        16.0f,                      // Specular Phong - concentration of specular light
        1.0f,                       // Constant Value for Attenuation
        0.35f,                      // Linear Value for Attenuation
        0.44f                       // Quadratic Value for Attenuation
    );
    
    /* DIRECTIONAL LIGHT (4, 11, -3) */
    DirectionalLight directionalLight(
        glm::vec3(4, 11, -3),     // Light Direction - Emphasis on direction, it represents the vector direction of light; Not a position
        glm::vec3(1.0f, 1.0f, 1.0f), // Light Color - RGB lighting of light source
        1.0f,                   // Light Strength - intensity of diffuse light    
        glm::vec3(1.0f, 1.0f, 1.0f), // Ambient Color - RGB lighting of reflected or ambient light
        0.5f,                   // Ambient Strength - Intensity of reflected or ambient light
        glm::vec3(1.0f, 1.0f, 1.0f), // Specular Color - RGB lighting of specular light
        1.0f,                  // Specular Strength - intensity of specular light
        50.0f                  // Specular Phong - concentration of specular light
    );

    

    /* INITIALIZATION FOR CAMERA VARIABLES, LOCS, AND SHADER TEMP VARIABLES*/
    glm::mat4 projection_matrix, view_matrix;
    glm::vec3 cameraPos;
    GLuint shaderProgram;    
    unsigned int projectionLoc, viewLoc, transformationLoc;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                              

        /* USE SHADER 0 - LIT OBJECT SHADER */        
        shaderProgram = shaderPrograms[0];
        glUseProgram(shaderProgram);

        /* SET CAMERA TYPE - OBTAIN THE CORRECT VARIABLES FOR THE CAMERA
            1 - PERSPECTIVE
            2 - ORTHOGRAPHIC
        */
        switch (cameraType) {
            case 1:
                projection_matrix = perspectiveCamera.getProjectionMatrix();
                view_matrix = perspectiveCamera.getViewMatrix();
                cameraPos = perspectiveCamera.getCameraPos();

                /* UPDATE MOVEMENT OF MOUSE UPON VALUE CHANGE */
                if (isOffsetChanged) {
                    perspectiveCamera.updateMouse(xoffset, yoffset);
                    isOffsetChanged = false;
                }                                
                break;
            case 2:
                projection_matrix = orthoCamera.getProjectionMatrix();
                view_matrix = orthoCamera.getViewMatrix();
                cameraPos = orthoCamera.getCameraPos();
                break;
        }                        

        projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection_matrix));
        viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view_matrix));




        /* VARIABLES FOR OBJECT POSITION, ROTATION, SCALE - (TO BE USED LATER) */
        transformationLoc = glGetUniformLocation(shaderProgram, "transform"); // transform is the variable from sample.vert
        /* VARIABLES FOR TEXTURE*/
        GLuint tex0Address = glGetUniformLocation(shaderProgram, "tex0"); // Get the address
        glUniform1i(tex0Address, 0); // Comes from GL_TEXTURE 0


        /* OBJECTS TRANSFORMATION */
        /************ LIGHT OBJ MATRIX and SAVE POINT LIGHT ************/
        /* Base transformation matrix */
        glm::mat4 light_obj_matrix = glm::mat4(1.0f);
        glm::mat4 transform_matrix = glm::mat4(1.0f);

        transform_matrix = glm::rotate(transform_matrix, glm::radians(light_rot_x), glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f))); // X - Axis Rotation
        transform_matrix = glm::rotate(transform_matrix, glm::radians(light_rot_y), glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f))); // Y - Axis Rotation
        transform_matrix = glm::rotate(transform_matrix, glm::radians(light_rot_z), glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f))); // Z - Axis Rotation

        light_obj_matrix = glm::translate(light_obj_matrix, pointLight.lightPos); //Translate the light rotated matrix

        float l_scale_x, l_scale_y, l_scale_z;
        l_scale_x = l_scale_y = l_scale_z = 0.25f;
        light_obj_matrix = glm::scale(light_obj_matrix, glm::vec3(l_scale_x, l_scale_y, l_scale_z)); // Provide the necessary scale

        pointLight.lightPos = transform_matrix * glm::vec4(3.0f, 2.0f, 0.0f, 1.0f); // Save the position for lightPos for later - to synchronize with the position of the light box object


        /************ MODEL OBJ MATRIX ************/
        /* Base transformation matrix */
        glm::mat4 model_matrix = glm::mat4(1.0);

        /* POSITIONS */
        model_matrix = glm::translate(model_matrix, glm::vec3(0.0f, 0.0f, 0.0f));

        /* SCALING */
        float m_scale_x, m_scale_y, m_scale_z;
        m_scale_x = m_scale_y = m_scale_z = 0.003f;
        model_matrix = glm::scale(model_matrix, glm::vec3(m_scale_x, m_scale_y, m_scale_z));

        model_matrix = glm::rotate(model_matrix, glm::radians(model_rot_x), glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f))); // X - Axis Rotation
        model_matrix = glm::rotate(model_matrix, glm::radians(model_rot_y), glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f))); // Y - Axis Rotation
        model_matrix = glm::rotate(model_matrix, glm::radians(model_rot_z), glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f))); // Z - Axis Rotation







        /* VARIABLES FOR LIGHTING */
        /************ DIRECTIONAL LIGHT ************/

        /* SET CONTROL OVER DIRECTIONAL LIGHT STRENGTH */
        if (isDLightChanged) {
            /* MAX FUNCTION IN ORDER TO PREVENT MODIFICATION OF LIGHT INTENSITY TO GO TO NEGATIVE VALUES */
            directionalLight.lightStr = std::max(d_light_intensity + directionalLight.lightStr, 0.0f);
            directionalLight.ambientStr = std::max(d_light_intensity + directionalLight.ambientStr, 0.0f);
            directionalLight.specStr = std::max(d_light_intensity + directionalLight.specStr, 0.0f);
            isDLightChanged = false;
        }        

        // LIGHT POSITION AND COLOR
        unsigned int directionalLightPosLoc = glGetUniformLocation(shaderProgram, "directionalLightPos");     // Light Pos
        glUniform3fv(directionalLightPosLoc, 1, glm::value_ptr(directionalLight.direction));
        unsigned int directionalLightColorLoc = glGetUniformLocation(shaderProgram, "directionalLightColor"); // Light Color
        glUniform3fv(directionalLightColorLoc, 1, glm::value_ptr(directionalLight.lightColor));
        unsigned int directionalLightStrLoc = glGetUniformLocation(shaderProgram, "directionalLightStr"); // Light Color
        glUniform1f(directionalLightStrLoc, directionalLight.lightStr);

        // AMBIENT STRENGTH AND COLOR
        unsigned int directionalAmbientStrLoc = glGetUniformLocation(shaderProgram, "directionalAmbientStr");     // Ambient Intensity
        glUniform1f(directionalAmbientStrLoc, directionalLight.ambientStr);
        unsigned int directionalAmbientColorLoc = glGetUniformLocation(shaderProgram, "directionalAmbientColor"); // Ambient Color
        glUniform3fv(directionalAmbientColorLoc, 1, glm::value_ptr(directionalLight.ambientColor));

        // CAMERA POSITION, SPECULAR STRENGTH, AND SPECULAR PHONG
        unsigned int directionalSpecStrLoc = glGetUniformLocation(shaderProgram, "directionalSpecStr");           // Specular Intensity
        glUniform1f(directionalSpecStrLoc, directionalLight.specStr);
        unsigned int directionalSpecColorLoc = glGetUniformLocation(shaderProgram, "directionalSpecColor");       // Specular Color
        glUniform3fv(directionalSpecColorLoc, 1, glm::value_ptr(directionalLight.specColor));
        unsigned int directionalSpecPhongLoc = glGetUniformLocation(shaderProgram, "directionalSpecPhong");       // Specular Phong
        glUniform1f(directionalSpecPhongLoc, directionalLight.specPhong);




        /************ POINT LIGHT ************/
        /* SET CONTROL OVER DIRECTIONAL LIGHT STRENGTH */
        if (isPLightChanged) {
            /* MAX FUNCTION IN ORDER TO PREVENT MODIFICATION OF LIGHT INTENSITY TO GO TO NEGATIVE VALUES */
            pointLight.lightStr = std::max(p_light_intensity + pointLight.lightStr, 0.0f);
            pointLight.ambientStr = std::max(p_light_intensity + pointLight.ambientStr, 0.0f);
            pointLight.specStr = std::max(p_light_intensity + pointLight.specStr, 0.0f);
            isPLightChanged = false;
        }        

        /* SET CONTROL OVER POINT LIGHT RGB */
        pointLight.lightColor = control_rgb;
        pointLight.ambientColor = control_rgb;
        pointLight.specColor = control_rgb;

        // LIGHT POSITION AND COLOR
        unsigned int pointLightPosLoc = glGetUniformLocation(shaderProgram, "pointLightPos");     // Light Pos
        glUniform3fv(pointLightPosLoc, 1, glm::value_ptr(pointLight.lightPos));
        unsigned int pointLightColorLoc = glGetUniformLocation(shaderProgram, "pointLightColor"); // Light Color
        glUniform3fv(pointLightColorLoc, 1, glm::value_ptr(pointLight.lightColor));
        unsigned int pointLightStrLoc = glGetUniformLocation(shaderProgram, "pointLightStr"); // Light Color
        glUniform1f(pointLightStrLoc, pointLight.lightStr);

        // AMBIENT STRENGTH AND COLOR
        unsigned int pointAmbientStrLoc = glGetUniformLocation(shaderProgram, "pointAmbientStr");     // Ambient Intensity
        glUniform1f(pointAmbientStrLoc, pointLight.ambientStr);
        unsigned int pointAmbientColorLoc = glGetUniformLocation(shaderProgram, "pointAmbientColor"); // Ambient Color
        glUniform3fv(pointAmbientColorLoc, 1, glm::value_ptr(pointLight.ambientColor));

        // CAMERA POSITION, SPECULAR STRENGTH, AND SPECULAR PHONG
        unsigned int cameraPosLoc = glGetUniformLocation(shaderProgram, "cameraPos");       // Camera Position
        glUniform3fv(cameraPosLoc, 1, glm::value_ptr(cameraPos));
        unsigned int pointSpecStrLoc = glGetUniformLocation(shaderProgram, "pointSpecStr");           // Specular Intensity
        glUniform1f(pointSpecStrLoc, pointLight.specStr);
        unsigned int pointSpecColorLoc = glGetUniformLocation(shaderProgram, "pointSpecColor");       // Specular Color
        glUniform3fv(pointSpecColorLoc, 1, glm::value_ptr(pointLight.specColor));
        unsigned int pointSpecPhongLoc = glGetUniformLocation(shaderProgram, "pointSpecPhong");       // Specular Phong
        glUniform1f(pointSpecPhongLoc, pointLight.specPhong);

        // ATTENUATION
        unsigned int pointConstantLoc = glGetUniformLocation(shaderProgram, "constant");       // Constant
        glUniform1f(pointConstantLoc, pointLight.constant);
        unsigned int pointLinearLoc = glGetUniformLocation(shaderProgram, "linear");       // Linear
        glUniform1f(pointLinearLoc, pointLight.linear);
        unsigned int pointQuadraticLoc = glGetUniformLocation(shaderProgram, "quadratic");       // Quadratic
        glUniform1f(pointQuadraticLoc, pointLight.quadratic);

        
        /* MODEL OBJECT ITEM - VAO0 and TEXTURE0 */
        // DRAW THE TEXTURE0
        glBindTexture(GL_TEXTURE_2D, texture); // Call OpenGL we're using that texture
        // MODEL OBJECT TRANSFORM MATRIX
        glUniformMatrix4fv(transformationLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));
        // DRAW OBJECT0 
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / 8); // divided by 8 to get the number of vertices to draw


        /* USE SHADER 1 - UNLIT OBJECT SHADER */
        shaderProgram = shaderPrograms[1];
        glUseProgram(shaderProgram);

        unsigned int colorLoc = glGetUniformLocation(shaderProgram, "color_rgba");
        glUniform4f(colorLoc, control_rgb.x, control_rgb.y, control_rgb.z, 1.0f);
        transformationLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformationLoc, 1, GL_FALSE, glm::value_ptr(light_obj_matrix));
        projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection_matrix));
        viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view_matrix));
        
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
    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);
       
    glfwTerminate();
    return 0;
}