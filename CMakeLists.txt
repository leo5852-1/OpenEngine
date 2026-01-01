# CMake minimum version requirement
cmake_minimum_required(VERSION 3.10)

# fix encoding to UTF-8 for MSVC compiler
if(MSVC)
    add_compile_options(/utf-8)
endif()

# project info
project(
  OpenEngine
  VERSION 0.1.0
  DESCRIPTION "OpenGL사용 3D엔진 프로젝트"
)
set(CMAKE_CXX_STANDARD 17)

# 1.Build GLFW library(exclude doc and examples)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(dependencies/glfw-3.4)

# 2.GLEW include header files path
include_directories(dependencies/glew-2.1.0/include)
# define GLEW to use as static library
add_definitions(-DGLEW_STATIC)

# 3.include header files path
include_directories(dependencies/glm)
include_directories(include)

# generate executable
add_executable (OpenEngine 
  dependencies/glew-2.1.0/src/glew.c
  src/main.cpp
  src/player.cpp
  include/player.h
  )

# link libraries
# opengl32는 윈도우 기본 시스템 라이브러리입니다. GLEW는 명시적으로 연결해주는 게 좋습니다.
target_link_libraries(OpenEngine glfw opengl32)

# 셰이더 파일을 소스 폴더(src)에서 빌드 폴더(build)로 복사
file(COPY ${CMAKE_SOURCE_DIR}/src/vShader.glsl DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/src/fShader.glsl DESTINATION ${CMAKE_BINARY_DIR})