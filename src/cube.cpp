#include "cube.h"

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
    glm::vec4( 0.0, 1.0, 1.0, 1.0 ),  // cyan
    glm::vec4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
    glm::vec4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
    glm::vec4(1.0, 0.0, 0.0, 1.0 ),  // red
    glm::vec4( 0.0, 1.0, 0.0, 1.0 ),  // green
    glm::vec4( 0.0, 0.0, 1.0, 1.0 ),  // blue
    glm::vec4( 1.0, 1.0, 1.0, 1.0 )  // white
};

Cube::Cube(unsigned int programID){
    this->modelLoc = glGetUniformLocation(programID, "model");
    
    colorcube();
    
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

void Cube::draw(){
    glUniformMatrix4fv(this->modelLoc, 1, GL_FALSE, &this->modelMatrix[0][0]);

    glBindVertexArray(this->vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

// generate 12 triangles: 36 vertices and 36 colors
void Cube::colorcube() {
    index = 0;
    quad( 1, 0, 3, 2 );
    quad( 2, 3, 7, 6 );
    quad( 3, 0, 4, 7 );
    quad( 6, 5, 1, 2 );
    quad( 4, 5, 6, 7 );
    quad( 5, 4, 0, 1 );
}

// quad generates two triangles for each face and assigns colors
//    to the vertices
void Cube::quad( int a, int b, int c, int d ) {
    colors[index] = vertex_colors[a]; points[index] = vertices[a]; index++;
    colors[index] = vertex_colors[b]; points[index] = vertices[b]; index++;
    colors[index] = vertex_colors[c]; points[index] = vertices[c]; index++;
    colors[index] = vertex_colors[a]; points[index] = vertices[a]; index++;
    colors[index] = vertex_colors[c]; points[index] = vertices[c]; index++;
    colors[index] = vertex_colors[d]; points[index] = vertices[d]; index++;
}

void Cube::translate(glm::vec3 matrix) {
    // 현재 모델 행렬에 이동 변환을 누적 적용
    this->modelMatrix = glm::translate(this->modelMatrix, matrix);
}

void Cube::rotate(glm::vec3 matrix, float elapsedTime) {
    this->modelMatrix = glm::rotate(this->modelMatrix, elapsedTime, matrix);
}