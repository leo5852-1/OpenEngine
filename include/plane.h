#ifndef PLANE_HPP
#define PLANE_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

class Plane {
public:
    Plane(unsigned int);
    void draw();
    void colorPlane();
    void quad(int, int, int, int);
    void translate(glm::vec3);
    void scale(glm::vec3);
    void rotate(glm::vec3, float);

    glm::vec4 points[6]; // (2 triangles)(3 vertices/triangle)
    glm::vec4 colors[6];
    glm::mat4 modelMatrix = glm::mat4(1.0f); // initial model matrix
    unsigned int modelLoc; // model uniform variable location in shader

private:
    GLuint vao;
    GLuint vbo;
    int index = 0;
};

#endif