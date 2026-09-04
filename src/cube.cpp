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

Cube::Cube(unsigned int programID, glm::vec3 size){

    isStatic = true;
    setCollider(new BoxCollider(size));

    std::vector<glm::vec4> points;
    std::vector<glm::vec4> colors;
    points.reserve(36);
    colors.reserve(36);

    colorcube(points, colors);

    setupMesh(programID, points, colors);
}

// generate 12 triangles: 36 vertices and 36 colors
void Cube::colorcube(std::vector<glm::vec4>& points, std::vector<glm::vec4>& colors) {
    quad( points, colors, 1, 0, 3, 2 );
    quad( points, colors, 2, 3, 7, 6 );
    quad( points, colors, 3, 0, 4, 7 );
    quad( points, colors, 6, 5, 1, 2 );
    quad( points, colors, 4, 5, 6, 7 );
    quad( points, colors, 5, 4, 0, 1 );
}

// quad generates two triangles for each face and assigns colors
//    to the vertices
void Cube::quad( std::vector<glm::vec4>& points, std::vector<glm::vec4>& colors, int a, int b, int c, int d ) {
    colors.push_back(vertex_colors[a]); points.push_back(vertices[a]);
    colors.push_back(vertex_colors[b]); points.push_back(vertices[b]);
    colors.push_back(vertex_colors[c]); points.push_back(vertices[c]);
    colors.push_back(vertex_colors[a]); points.push_back(vertices[a]);
    colors.push_back(vertex_colors[c]); points.push_back(vertices[c]);
    colors.push_back(vertex_colors[d]); points.push_back(vertices[d]);
}
