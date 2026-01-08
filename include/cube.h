#ifndef CUBE_HPP
#define CUBE_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

class Cube {
public:
    Cube(unsigned int);
    void draw();
    void colorcube();
    void quad(int, int, int, int);
    void translate(glm::vec3);
    void rotate(glm::vec3, float);

    glm::vec4 points[36]; // (6 faces)(2 triangles/face)(3 vertices/triangle)
    glm::vec4 colors[36];
    glm::mat4 modelMatrix = glm::mat4(1.0f); // initial model matrix
    unsigned int modelLoc; // 셰이더의 model uniform 위치 저장용

private:
    GLuint vao;
    GLuint vbo;
    int index = 0;
};

#endif