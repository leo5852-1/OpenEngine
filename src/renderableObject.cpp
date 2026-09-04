#include "renderableObject.h"

RenderableObject::~RenderableObject() {
    if (vbo) glDeleteBuffers(1, &vbo);
    if (vao) glDeleteVertexArrays(1, &vao);
}

void RenderableObject::setupMesh(unsigned int programID, const std::vector<glm::vec4>& points, const std::vector<glm::vec4>& colors) {
    this->modelLoc = glGetUniformLocation(programID, "model");
    this->vertexCount = (int)points.size();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    size_t pointsBytes = points.size() * sizeof(glm::vec4);
    size_t colorsBytes = colors.size() * sizeof(glm::vec4);
    glBufferData(GL_ARRAY_BUFFER, pointsBytes + colorsBytes, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, pointsBytes, points.data());
    glBufferSubData(GL_ARRAY_BUFFER, pointsBytes, colorsBytes, colors.data());

    GLuint vPosition = glGetAttribLocation(programID, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    GLuint vColor = glGetAttribLocation(programID, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)pointsBytes);

    glBindVertexArray(0);
}

void RenderableObject::draw() {
    glUniformMatrix4fv(this->modelLoc, 1, GL_FALSE, &this->modelMatrix[0][0]);

    glBindVertexArray(this->vao);
    glDrawArrays(GL_TRIANGLES, 0, this->vertexCount);
    glBindVertexArray(0);
}

void RenderableObject::translate(glm::vec3 vec) {
    // 현재 모델 행렬에 이동 변환을 누적 적용
    this->modelMatrix = glm::translate(this->modelMatrix, vec);
    this->position += vec;
}

void RenderableObject::rotate(glm::vec3 axis, float elapsedTime) {
    this->modelMatrix = glm::rotate(this->modelMatrix, elapsedTime, axis);
}
