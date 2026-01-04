// ALWAYS INCLUDE GLEW -> GLFW IN ORDER //
#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

#include <player.h>
#include <shader.h>

#define BORDER_LEFT 0
#define BORDER_RIGHT 1280
#define BORDER_DOWN 0
#define BORDER_UP 720

#define MAP_ROWS 8
#define MAP_COLS 8
#define MAP_SIZE 64

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

// Matrix transformation
GLuint pvmMatrixID;
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
//for drawing cube
glm::vec4 vertices[8] = {
    glm::vec4( -0.5, -0.5,  0.5, 1.0 ),
    glm::vec4( -0.5,  0.5,  0.5, 1.0 ),
    glm::vec4(  0.5,  0.5,  0.5, 1.0 ),
    glm::vec4(  0.5, -0.5,  0.5, 1.0 ),
    glm::vec4( -0.5, -0.5, -0.5, 1.0 ),
    glm::vec4( -0.5,  0.5, -0.5, 1.0 ),
    glm::vec4(  0.5,  0.5, -0.5, 1.0 ),
    glm::vec4(  0.5, -0.5, -0.5, 1.0 )
};

// RGBA olors
glm::vec4 vertex_colors[8] = {
    glm::vec4( 0.0, 0.0, 0.0, 1.0 ),  // black
    glm::vec4( 0.0, 1.0, 1.0, 1.0 ),   // cyan
    glm::vec4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
    glm::vec4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
    glm::vec4(1.0, 0.0, 0.0, 1.0 ),  // red
    glm::vec4( 0.0, 1.0, 0.0, 1.0 ),  // green
    glm::vec4( 0.0, 0.0, 1.0, 1.0 ),  // blue
    glm::vec4( 1.0, 1.0, 1.0, 1.0 )
};  // white

const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

glm::vec4 points[NumVertices];
glm::vec4 colors[NumVertices];
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

    // The main loop
    while(!glfwWindowShouldClose(window))
    {
        // delta time calculation
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        processInput(window);

        glClearColor(0.1f, 0.7f, 0.2f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f);
        // 큐브가 보이도록 계속 회전시켜보기
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f)); 
        glm::mat4 pvm = projectMat * viewMat * model;
        glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvm[0][0]);
        
        //mainLoopEvent();
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

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
    
    colorcube();
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);    

    GLuint vPositon = glGetAttribLocation(shader.programID, "vPosition");
    glEnableVertexAttribArray(vPositon);
    glVertexAttribPointer(vPositon, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    GLuint vColor = glGetAttribLocation(shader.programID, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, ((GLvoid*)sizeof(points)) );

    pvmMatrixID = glGetUniformLocation(shader.programID, "mPVM");
    projectMat = glm::perspective(glm::radians(65.0f), 1.0f, 0.1f, 100.0f);
    viewMat = glm::lookAt(glm::vec3(0, 0, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    framebuffer_size_callback(NULL, SCR_WIDTH, SCR_HEIGHT);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.7f, 0.2f, 0.0f);
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
    float speed = player.mvSpeed;
    // W
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && player.y < BORDER_UP)
        player.y += speed * deltaTime;
    // S
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && player.y > BORDER_DOWN)
        player.y -= speed * deltaTime;
    // A
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && player.x > BORDER_LEFT)
        player.x -= speed * deltaTime;
    // D
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && player.x < BORDER_RIGHT)
        player.x += speed * deltaTime;
    // Q
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        player.angle += 180.0f * deltaTime;
    // E
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        player.angle -= 180.0f * deltaTime;
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

// quad generates two triangles for each face and assigns colors
//    to the vertices
int Index = 0;
void
quad( int a, int b, int c, int d )
{
    colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
    colors[Index] = vertex_colors[b]; points[Index] = vertices[b]; Index++;
    colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
    colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
    colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
    colors[Index] = vertex_colors[d]; points[Index] = vertices[d]; Index++;
}

//----------------------------------------------------------------------------

// generate 12 triangles: 36 vertices and 36 colors
void
colorcube()
{
    quad( 1, 0, 3, 2 );
    quad( 2, 3, 7, 6 );
    quad( 3, 0, 4, 7 );
    quad( 6, 5, 1, 2 );
    quad( 4, 5, 6, 7 );
    quad( 5, 4, 0, 1 );
}

//----------------------------------------------------------------------------