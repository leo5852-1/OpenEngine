// ALWAYS INCLUDE GLEW -> GLFW IN ORDER //
#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/rotate_vector.hpp>
#include "glm/gtx/transform.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

#include <player.h>
#include <shader.h>
#include <cube.h>

#define BORDER_LEFT 0
#define BORDER_RIGHT 1280
#define BORDER_DOWN 0
#define BORDER_UP 720

#define MAP_ROWS 8
#define MAP_COLS 8
#define MAP_SIZE 64

#define CLEAR_COLOR 0.31f, 0.73f, 0.87f, 0.0f
#define CAMERA_SPEED 2.5f

using namespace std;

void init();
void onKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
void mainLoopEvent();

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void drawMap2D();
void drawPlayer();

void quad( int a, int b, int c, int d );
void colorcube();

//////// Global variables ////////
// screen size
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// Shader linked program
Shader shader;
// Vertex array object
GLuint vao;

// Camera matrices
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// Matrix transformation
//GLuint pvmMatrixID; //removed to calculate in shader
glm::mat4 modelMat;
glm::mat4 projectMat;
glm::mat4 viewMat;

Player player;
float deltaTime = 0.0f; 
float lastFrame = 0.0f;
int map[64] = 
{
1,1,1,1,1,1,1,1,
1,0,1,0,0,0,0,1,
1,0,1,0,0,0,0,1,
1,0,1,0,0,0,0,1,
1,0,0,0,0,0,0,1,
1,0,0,0,0,1,0,1,
1,0,0,0,0,0,0,1,
1,1,1,1,1,1,1,1
};
//////////////////////////////////

////////// MAIN function - program entry point //////////
int main() {
    GLFWwindow* window;
    
    if(!glfwInit()){
        cout << "GLFW init failed";
        return -1;
    }

    window = glfwCreateWindow(1280, 720, "OpenEngine", NULL, NULL);
    if(!window){
        glfwTerminate();
        cout << "Window creation failed";
        return -1;
    }

    glfwMakeContextCurrent(window);

    // load OpenGL function pointers by glew
    glewInit();

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, onKeyEvent);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Load and link shaders into program and use it
    shader.setup("../src/vShader.glsl", "../src/fShader.glsl");
    shader.use();

    init();

    Cube cube1(shader.programID);
    Cube cube2(shader.programID);
    cube2.translate(glm::vec3(1.5f, 0.0f, 0.0f));

    // The main loop
    while(!glfwWindowShouldClose(window))
    {
        // delta time calculation
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //proccess inputs
        processInput(window);
        // clear the frame and buffer
        glClearColor(CLEAR_COLOR);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // calculate view matrix
        viewMat = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        // rotate cube
        modelMat = glm::mat4(1.0f);
        modelMat = glm::rotate(modelMat, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f)); 
        
        // set MVP matrices
        glUniformMatrix4fv(glGetUniformLocation(shader.programID, "model"), 1, GL_FALSE, &modelMat[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shader.programID, "view"), 1, GL_FALSE, &viewMat[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shader.programID, "projection"), 1, GL_FALSE, &projectMat[0][0]);

        //mainLoopEvent();
        cube1.rotate(glm::vec3(0.5f, 1.0f, 0.0f), deltaTime);
        cube1.draw();
        
        cube2.rotate(glm::vec3(0.5f, 1.0f, 0.0f), deltaTime);
        cube2.draw();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
////////////////////////////////////////////////////////////

// initialize function
void init(){
    //glfwSetTime(0.0);

    // initialize MVP matrices
    projectMat = glm::perspective(glm::radians(65.0f), 1.0f, 0.1f, 100.0f);
    viewMat = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    modelMat = glm::mat4(1.0f);
    
    framebuffer_size_callback(NULL, SCR_WIDTH, SCR_HEIGHT);

    glEnable(GL_DEPTH_TEST);
    glClearColor(CLEAR_COLOR);
}

// Main loop function
void mainLoopEvent(){
    //player.update(deltaTime);
    drawMap2D();
    drawPlayer();
}

// onKeyEvent() is called only once while pressed
// processInput() 
// Keyboard process function
void processInput(GLFWwindow* window){
    float cameraSpeed = CAMERA_SPEED * deltaTime;
    float rotateSpeed = CAMERA_SPEED * deltaTime;
    // W
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    // S
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    // A
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
    // D
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
    // Q
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cameraFront = glm::rotate(cameraFront, cameraSpeed, cameraUp);
    // E
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        cameraFront = glm::rotate(cameraFront, -cameraSpeed, cameraUp);
}

// Keyboard event function
void onKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods) {
    switch(key){
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, true);
            break;
    }
}

void drawMap2D(){
    int x,y,xo,yo;
    for(y=0;y<MAP_ROWS;y++) {
        for(x=0;x<MAP_COLS;x++) {
            if(map[ y * MAP_COLS + x]==1) 
                glColor3f(1,1,1);
            else 
                glColor3f(0,0,0);
            xo = x * MAP_SIZE; yo = y * MAP_SIZE;
            glBegin(GL_QUADS); 
            glVertex2i( 0        + xo+1, 0        + yo+1); 
            glVertex2i( 0        + xo+1, MAP_SIZE + yo-1); 
            glVertex2i( MAP_SIZE + xo-1, MAP_SIZE + yo-1);  
            glVertex2i( MAP_SIZE + xo-1, 0        + yo+1); 
            glEnd();
        } 
    }
}

void drawPlayer(){
    glPushMatrix();

    glTranslatef(player.x, player.y, 0.0f);
    glRotatef(player.angle, 0.0f, 0.0f, 1.0f);

    float size = 10.0f; // 諛섏?由?媛숈? ?먮굦 (珥??ш린??20x20)
    glColor3f(1, 0, 0); // 鍮④컙??
    glBegin(GL_QUADS);
        glVertex2f(-size, -size); // 醫뚯륫 ?섎떒
        glVertex2f( size, -size); // ?곗륫 ?섎떒
        glVertex2f( size,  size); // ?곗륫 ?곷떒
        glVertex2f(-size,  size); // 醫뚯륫 ?곷떒
    glEnd();

    glPopMatrix();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

    if (height == 0) height = 1;

    float ratio = (float)width / (float)height;
    projectMat = glm::perspective(glm::radians(65.0f), ratio, 0.1f, 100.0f);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	;
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	;
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    ;
}


//----------------------------------------------------------------------------
