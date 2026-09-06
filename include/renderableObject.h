#ifndef RENDERABLEOBJECT_HPP
#define RENDERABLEOBJECT_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "gameObject.h"

// GameObject에 실제로 화면에 그려지는 메시(vao/vbo/모델행렬)를 더한 class
// Cube, Plane처럼 렌더링되는 오브젝트는 이 클래스를 상속받음
class RenderableObject : public GameObject {
public:
    virtual ~RenderableObject();

    void draw();
    void translate(glm::vec3 vec);
    void rotate(glm::vec3 axis, float elapsedTime);

    // localMatrix는 rotation/scale만 반영하고, position은 따로 유지 
    // 최종 Model Matrix은 draw()에서 translate(position) * localMatrix로 조립한다.
    glm::mat4 localMatrix = glm::mat4(1.0f);

protected:
    // interleaved position+color 정점 데이터를 업로드하고 vao/vbo/attrib를 설정한다.
    void setupMesh(unsigned int programID, const std::vector<glm::vec4>& points, const std::vector<glm::vec4>& colors);

    unsigned int modelLoc = 0; // model uniform 위치

private:
    GLuint vao = 0;
    GLuint vbo = 0;
    int vertexCount = 0;
};

#endif
