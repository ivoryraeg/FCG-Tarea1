// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
//#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <time.h>
#include <chrono>

#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
using namespace glm;

#include "common/shader.hpp"

double timeToShader = 3;

int screen = 1;
// This will identify our vertex buffer
GLuint vertexbuffer[2];

GLuint programID;
GLuint programID2;

class Triangle
{
    // An array of 3 vectors which represents 3 vertices
public:
    glm::vec3 vertices[3] = {
        vec3(-1, -1, 0),
        vec3(1, -1, 0),
        vec3(0, 1, 0)};
    quat rot = quat(vec3(0,0,0));
    vec3 scale = vec3(1,1,1);
    glm::vec3 pos = vec3(0,0,0);
    void Rotate(vec3 eulerAngles){        
        rot = rot*quat(eulerAngles);
    } 
    void PassToBuffer(GLfloat *vertexB){
        int j = 0;
        for (int i = 0; i < 9; i += 3)
        {
            vec3 vector = vertices[j]*scale*rot;
            vector += pos;
            vertexB[i] = vector.x;
            vertexB[i + 1] = vector.y;
            vertexB[i + 2] = vector.z;
            j++;
        }
    }
};

class Square{
    public:
    glm::vec3 vertices[36] = {
        vec3(-1.0f,-1.0f,-1.0f), // triangle 1 : begin
        vec3(-1.0f,-1.0f, 1.0f),
        vec3(-1.0f, 1.0f, 1.0f), // triangle 1 : end
        vec3(1.0f, 1.0f,-1.0f), // triangle 2 : begin
        vec3(-1.0f,-1.0f,-1.0f),
        vec3(-1.0f, 1.0f,-1.0f), // triangle 2 : end
        vec3(1.0f,-1.0f, 1.0f),
        vec3(-1.0f,-1.0f,-1.0f),
        vec3(1.0f,-1.0f,-1.0f),
        vec3(1.0f, 1.0f,-1.0f),
        vec3(1.0f,-1.0f,-1.0f),
        vec3(-1.0f,-1.0f,-1.0f),
        vec3(-1.0f,-1.0f,-1.0f),
        vec3(-1.0f, 1.0f, 1.0f),
        vec3(-1.0f, 1.0f,-1.0f),
        vec3(1.0f,-1.0f, 1.0f),
        vec3(-1.0f,-1.0f, 1.0f),
        vec3(-1.0f,-1.0f,-1.0f),
        vec3(-1.0f, 1.0f, 1.0f),
        vec3(-1.0f,-1.0f, 1.0f),
        vec3(1.0f,-1.0f, 1.0f),
        vec3(1.0f, 1.0f, 1.0f),
        vec3(1.0f,-1.0f,-1.0f),
        vec3(1.0f, 1.0f,-1.0f),
        vec3(1.0f,-1.0f,-1.0f),
        vec3(1.0f, 1.0f, 1.0f),
        vec3(1.0f,-1.0f, 1.0f),
        vec3(1.0f, 1.0f, 1.0f),
        vec3(1.0f, 1.0f,-1.0f),
        vec3(-1.0f, 1.0f,-1.0f),
        vec3(1.0f, 1.0f, 1.0f),
        vec3(-1.0f, 1.0f,-1.0f),
        vec3(-1.0f, 1.0f, 1.0f),
        vec3(1.0f, 1.0f, 1.0f),
        vec3(-1.0f, 1.0f, 1.0f),
        vec3(1.0f,-1.0f, 1.0f)
    };
    vec3 squareMove = vec3(0,0,0);
    float timer = 3;
    int direction = 1;
    quat squareRot = quat(vec3(0,0,0));
    void Rotate(vec3 eulerAngles){        
        squareRot = squareRot*quat(eulerAngles);
    } 
    vec3 squareScale = vec3(1,1,1);
    void PassToBuffer(GLfloat *vertexB){

        for(int i = 0; i < 36 ; i++){
            vec3 aux = vertices[i];
            aux = aux * squareScale;
            aux = aux * squareRot;
            aux = aux + squareMove;
            vertexB[i*3] = aux.x;
            vertexB[i*3+1] = aux.y;
            vertexB[i*3+2] = aux.z;
        }
    }
};

Triangle triangle1, triangle2;
Square square1;

static GLfloat g_vertex_buffer_data1[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.0f, 1.0f, 0.0f
    };
static GLfloat g_vertex_buffer_data2[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.0f, 1.0f, 0.0f
    };

static GLfloat g_vertex_buffer_data[] = {//Triangulos que componen el cuadrado
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};    
// One color for each vertex. They were generated randomly.
static const GLfloat g_color_buffer_data[] = {
    0.583f,  0.771f,  0.014f,
    0.609f,  0.115f,  0.436f,
    0.327f,  0.483f,  0.844f,
    0.822f,  0.569f,  0.201f,
    0.435f,  0.602f,  0.223f,
    0.310f,  0.747f,  0.185f,
    0.597f,  0.770f,  0.761f,
    0.559f,  0.436f,  0.730f,
    0.359f,  0.583f,  0.152f,
    0.483f,  0.596f,  0.789f,
    0.559f,  0.861f,  0.639f,
    0.195f,  0.548f,  0.859f,
    0.014f,  0.184f,  0.576f,
    0.771f,  0.328f,  0.970f,
    0.406f,  0.615f,  0.116f,
    0.676f,  0.977f,  0.133f,
    0.971f,  0.572f,  0.833f,
    0.140f,  0.616f,  0.489f,
    0.997f,  0.513f,  0.064f,
    0.945f,  0.719f,  0.592f,
    0.543f,  0.021f,  0.978f,
    0.279f,  0.317f,  0.505f,
    0.167f,  0.620f,  0.077f,
    0.347f,  0.857f,  0.137f,
    0.055f,  0.953f,  0.042f,
    0.714f,  0.505f,  0.345f,
    0.783f,  0.290f,  0.734f,
    0.722f,  0.645f,  0.174f,
    0.302f,  0.455f,  0.848f,
    0.225f,  0.587f,  0.040f,
    0.517f,  0.713f,  0.338f,
    0.053f,  0.959f,  0.120f,
    0.393f,  0.621f,  0.362f,
    0.673f,  0.211f,  0.457f,
    0.820f,  0.883f,  0.371f,
    0.982f,  0.099f,  0.879f
};

void rotateObject(double deltaTime, GLFWwindow *window, std::string sceneName){

    std::string currentScene = sceneName;
    if(currentScene == "scene2"){
        if(glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS){
            triangle1.Rotate(vec3(deltaTime,0,0));
        }   
        if(glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS){
            triangle1.Rotate(vec3(0,deltaTime,0));
        }  
        if(glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS){
            triangle1.Rotate(vec3(0,0,deltaTime));
        } 
    }
    if(currentScene == "scene3"){
        if(glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS){
            square1.Rotate(vec3(deltaTime,0,0));
        }   
        if(glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS){
            square1.Rotate(vec3(0,deltaTime,0));
        }  
        if(glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS){
            square1.Rotate(vec3(0,0,deltaTime));
        } 
    }
}

void scaleObject(double deltaTime, GLFWwindow *window, std::string sceneName)
{
    std::string currentScene = sceneName;
    if(currentScene == "scene2"){
         if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        {
            triangle1.scale.x += deltaTime;
        }
        if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
        {
            triangle1.scale.y += deltaTime;
        }
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        {
            triangle1.scale.z += deltaTime;
        } 
    }
    if(currentScene == "scene3"){
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        {
            square1.squareScale.x += deltaTime;
        }
        if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
        {
            square1.squareScale.y += deltaTime;
        }
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        {
            square1.squareScale.z += deltaTime;
        } 
    }
}


void Scene1(double deltaTime,GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS){
        if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE)
            screen = 1;
    }
}

void Scene2(double deltaTime, GLFWwindow *window)
{
    // Draw triangle...

    glUseProgram(programID);
    // 1st attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[0]);     
    glVertexAttribPointer(
        0,        // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,        // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0,        // stride
        (void *)0 // array buffer offset
    );
    if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS){//Color rojo
        // update the color
        //Obtengo ubicasi??n del color
        int vertexColorLocation = glGetUniformLocation(programID, "ourColor");//Se pasa el valor que retorna y el nombre de la variable
        glUniform3f(vertexColorLocation, 1.0f, 0.0f, 0.0f);
    }if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS){//Color verde
        // update the color
        //Obtengo ubicasi??n del color
        int vertexColorLocation = glGetUniformLocation(programID, "ourColor");//Se pasa el valor que retorna y el nombre de la variable
        glUniform3f(vertexColorLocation, 0.0f, 1.1f, 0.0f);
    }if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS){//color azul
        // update the color
        //Obtengo ubicasi??n del color
        int vertexColorLocation = glGetUniformLocation(programID, "ourColor");//Se pasa el valor que retorna y el nombre de la variable
        glUniform3f(vertexColorLocation, 0.0f, 0.0f, 1.0f);
    }

    triangle1.Rotate(vec3(deltaTime/2, deltaTime/2, deltaTime/2));

    rotateObject(deltaTime,window, "scene2");
    scaleObject(deltaTime,window, "scene2");
    glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle

    glDisableVertexAttribArray(0);



    // Don't forget to #include <glm/gtc/quaternion.hpp> and <glm/gtx/quaternion.hpp>

    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        triangle1.pos += vec3(-deltaTime,0,0);
    }
    if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS){
        if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE)
            screen = 2;
    }

    triangle1.PassToBuffer(g_vertex_buffer_data1);

    
}

void Scene3(double deltaTime, GLFWwindow *window){
    
    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);

    if (square1.timer <= 0) //temporizador. Cada 3 segundos cambia la direcci??n del mobimiento.
    {
        square1.timer = 3;
        square1.direction *= -1;
    }
    else
    {
        square1.timer -= deltaTime;
    }
    vec3 move = vec3(0,0,deltaTime * square1.direction);
    square1.squareMove += move;

    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glVertexAttribPointer(
        0,                                // attribute. No particular reason for 1, but must match the layout in the shader.
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );
    rotateObject(deltaTime,window, "scene3");
    scaleObject(deltaTime,window, "scene3");
    glDrawArrays(GL_TRIANGLES, 0, 12*3);
    glDisableVertexAttribArray(0);
    square1.PassToBuffer(g_vertex_buffer_data);
}


int main()
{
    // Projection matrix : 45?? Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float) 768 / (float)768, 0.1f, 100.0f);
    
    // Or, for an ortho camera :
    //glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
    
    // Camera matrix
    glm::mat4 View = glm::lookAt(
        glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
        glm::vec3(0,0,0), // and looks at the origin
        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
        );
    
    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);
    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around

    float y = 0;
    // Initialise GLFW
    glewExperimental = true; // Needed for core profile
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }
    glfwWindowHint(GLFW_SAMPLES, 4);               // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed **
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL **

    // Open a window and create its OpenGL context
    GLFWwindow *window; // (In the accompanying source code, this variable is global for simplicity)
    window = glfwCreateWindow(768, 768, "Tutorial 01", NULL, NULL);
    if (window == NULL)
    {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // Initialize GLEW
    glewExperimental = true;        // Needed in core profile
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    GLuint VertexArrayID[2];
    glGenVertexArrays(2, VertexArrayID);
    glBindVertexArray(VertexArrayID[0]);//**
    glBindVertexArray(VertexArrayID[1]);//**

    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(2, vertexbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    //glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[0]);//**    
    // Give our vertices to OpenGL.
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[0]);
    glNamedBufferData(VertexArrayID[0], sizeof(g_vertex_buffer_data1), g_vertex_buffer_data1, GL_STATIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[1]);    
    glNamedBufferData(VertexArrayID[1], sizeof(g_vertex_buffer_data2), g_vertex_buffer_data2, GL_STATIC_DRAW);
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("MyVertex.shader", "MyFragment.shader");
    programID2 = LoadShaders("MyVertex2.shader", "MyFragment.shader");
    // Use our shader
    glUseProgram(programID);


    auto t_start = std::chrono::high_resolution_clock::now();
    // the work...
    auto t_end = std::chrono::high_resolution_clock::now();

    double deltaTime = 0;
    triangle2.pos = vec3(0,0,-1.5);

    do
    {
        // Get a handle for our "MVP" uniform
        // Only during the initialisation
        GLuint MatrixID = glGetUniformLocation(programID, "MVP");
        
        // Send our transformation to the currently bound shader, in the "MVP" uniform
        // This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

        //getsTime Dif
        t_start = t_end;
        t_end = std::chrono::high_resolution_clock::now();

        deltaTime = std::chrono::duration<double>(t_end - t_start).count();
            
        timeToShader = 3;

        timeToShader -= deltaTime;
        if (timeToShader < 0)
        {
            timeToShader = 3;
            GLuint newProgramID = LoadShaders("MyVertex.shader", "MyFragment.shader");
            // Use our shader
            glUseProgram(newProgramID);

            glDeleteProgram(programID);
            programID = newProgramID;
        }

        // Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Draw through function
        if(screen == 0){
            Scene1(deltaTime, window);
        }else if(screen == 1){
            Scene2(deltaTime, window);
        }else if(screen == 2){
            Scene3(deltaTime, window);
        }


        glNamedBufferData(VertexArrayID[0], sizeof(g_vertex_buffer_data1), g_vertex_buffer_data1, GL_STATIC_DRAW);        
        glNamedBufferData(VertexArrayID[1], sizeof(g_vertex_buffer_data2), g_vertex_buffer_data2, GL_STATIC_DRAW);
        
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);
}