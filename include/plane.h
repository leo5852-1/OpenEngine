#ifndef PLANE_HPP
#define PLANE_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <aabb.h>

class Plane {
public:
    Plane(unsigned int);
    void draw();
    void colorPlane();
    void quad(int, int, int, int);
    void translate(glm::vec3);
    void scale(glm::vec3);
    void rotate(glm::vec3, float);
    AABB getAABB();

    glm::vec4 points[6]; // (2 triangles)(3 vertices/triangle)
    glm::vec4 colors[6];
    
    glm::mat4 modelMatrix = glm::mat4(1.0f); // initial model matrix
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 scaleSize = glm::vec3(1.0f);

    //TODO: change to use layout number set in shader
    unsigned int modelLoc; // model uniform variable location in shader

private:
    GLuint vao;
    GLuint vbo;
};

#endif