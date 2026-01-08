#include "plane.h"

glm::vec4 planeVertices[4] = {
    glm::vec4(-0.5f, 0.0f, -0.5f, 1.0f), // 0
    glm::vec4( 0.5f, 0.0f, -0.5f, 1.0f), // 1
    glm::vec4( 0.5f, 0.0f,  0.5f, 1.0f), // 2
    glm::vec4(-0.5f, 0.0f,  0.5f, 1.0f) // 3
    /*
    0---1
    | \ |
    3---2
    */
};

Plane::Plane(unsigned int programID){
    this->modelLoc = glGetUniformLocation(programID, "model");
    
    colorPlane();
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);    

    GLuint vPositon = glGetAttribLocation(programID, "vPosition");
    glEnableVertexAttribArray(vPositon);
    glVertexAttribPointer(vPositon, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    GLuint vColor = glGetAttribLocation(programID, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, ((GLvoid*)sizeof(points)) );
}

void Plane::draw(){
    glUniformMatrix4fv(this->modelLoc, 1, GL_FALSE, &this->modelMatrix[0][0]);

    glBindVertexArray(this->vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Plane::translate(glm::vec3 matrix) {
    this->modelMatrix = glm::translate(this->modelMatrix, matrix);
}

void Plane::scale(glm::vec3 matrix) {
    this->modelMatrix = glm::scale(this->modelMatrix, matrix);
}

void Plane::colorPlane(){
    for(int i=0; i<6; i++){
        colors[i] = glm::vec4(0.13f, 0.55f, 0.13f, 1.0f);  // grass green
    }
    int index = 0;
    points[index] = planeVertices[0]; index++;
    points[index] = planeVertices[2]; index++;
    points[index] = planeVertices[1]; index++;
    points[index] = planeVertices[0]; index++;
    points[index] = planeVertices[3]; index++;
    points[index] = planeVertices[2]; index++;
}