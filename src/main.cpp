// ALWAYS INCLUDE GLEW -> GLFW IN ORDER //
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
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

//////// Global variables ////////
// screen size
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

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

    float size = 10.0f; // 반지름 같은 느낌 (총 크기는 20x20)
    glColor3f(1, 0, 0); // 빨간색
    glBegin(GL_QUADS);
        glVertex2f(-size, -size); // 좌측 하단
        glVertex2f( size, -size); // 우측 하단
        glVertex2f( size,  size); // 우측 상단
        glVertex2f(-size,  size); // 좌측 상단
    glEnd();

    glPopMatrix();
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
    // W 키가 눌려있으면 위로
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && player.y < BORDER_UP)
        player.y += speed * deltaTime;
    // S 키가 눌려있으면 아래로
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && player.y > BORDER_DOWN)
        player.y -= speed * deltaTime;
    // A 키가 눌려있으면 왼쪽으로
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && player.x > BORDER_LEFT)
        player.x -= speed * deltaTime;
    // D 키가 눌려있으면 오른쪽으로
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && player.x < BORDER_RIGHT)
        player.x += speed * deltaTime;
    // Q to turn left
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        player.angle += 180.0f * deltaTime;
    // E to turn right
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

// initialize function
void init(){
    //glfwSetTime(0.0);
    player.setPos(320, 360); 
    
    framebuffer_size_callback(NULL, SCR_WIDTH, SCR_HEIGHT);
}

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

    init();

    Shader shader("../src/vShader.glsl", "../src/fShader.glsl");

    // The main loop
    while(!glfwWindowShouldClose(window))
    {
        // delta time calculation
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        processInput(window);

        glClearColor(0.1f, 0.7f, 0.2f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //mainLoopEvent();
        //player.update(deltaTime);
        drawMap2D();
        drawPlayer();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
////////////////////////////////////////////////////////////

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
// 1. 뷰포트(그림 그릴 영역) 설정
    glViewport(0, 0, width, height);

    // 2. 투영 행렬(Projection) 초기화 및 설정
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // 좌표계를 (0, 0) ~ (width, height)로 설정 (z축 범위 넉넉하게 -10 ~ 10)
    glOrtho(0.0, (double)width, 0.0, (double)height, -10.0, 10.0);

    // 3. 모델뷰 행렬(ModelView) 초기화
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
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