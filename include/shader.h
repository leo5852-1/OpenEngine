#ifndef SHADER_HPP
#define SHADER_HPP

#include <GL/glew.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>


using namespace std;

class Shader 
{
public:
    unsigned int programID;
    Shader(const char* vertexPath, const char* fragmentPath){
        // 1. Create shader object
        GLuint vShaderID = glCreateShader(GL_VERTEX_SHADER);
        GLuint fShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        
        // 2. read shader source codes from file
        string vShaderCode;
        ifstream vStream(vertexPath);
        if(vStream.is_open()){
            stringstream sstr;
            sstr << vStream.rdbuf();
            vShaderCode = sstr.str();
            vStream.close();
        }
        else{
            cout << "ERROR: Can't open file name \"" << vertexPath << "\"";
            return;
        }
        char const* vShaderCodePtr = vShaderCode.c_str();
        //cout << "-- vertex shader code --\n" << vShaderCode << endl;

        string fShaderCode;
        ifstream fStream(fragmentPath);
        if(fStream.is_open()){
            stringstream sstr;
            sstr << fStream.rdbuf();
            fShaderCode = sstr.str();
            fStream.close();
        }
        else{
            cout << "ERROR: Can't open file name \"" << fragmentPath << "\"";
            return;
        }
        char const* fShaderCodePtr = fShaderCode.c_str();
        //cout << "-- fragment shader code --\n" << fShaderCode << endl;

        // 3. vertex shader compile & error check
        GLint compileResult = GL_FALSE;
        int infoLogLength = 0;

        printf("\nCompiling Vertex Shader from \"%s\"\n", vertexPath);
        glShaderSource(vShaderID, 1, &vShaderCodePtr, NULL);
        glCompileShader(vShaderID);
        glGetShaderiv(vShaderID, GL_COMPILE_STATUS, &compileResult);
        if(compileResult == GL_FALSE){
            glGetShaderiv(vShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
            vector<char> vShaderErrorMsg(infoLogLength+1);
            glGetShaderInfoLog(vShaderID, infoLogLength, NULL, &vShaderErrorMsg[0]);
            cout << "Shader Compile Error:\n" << &vShaderErrorMsg[0] << endl;
        }
        else
            printf("Compile success\n");

        // 4. fragment shader compile & error check
        compileResult = GL_FALSE;
        infoLogLength = 0;

        printf("\nCompiling Fragment Shader from \"%s\"\n", fragmentPath);
        glShaderSource(fShaderID, 1, &fShaderCodePtr, NULL);
        glCompileShader(fShaderID);
        glGetShaderiv(fShaderID, GL_COMPILE_STATUS, &compileResult);
        if(compileResult == GL_FALSE){
            glGetShaderiv(fShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
            vector<char> fShaderErrorMsg(infoLogLength+1);
            glGetShaderInfoLog(fShaderID, infoLogLength, NULL, &fShaderErrorMsg[0]);
            cout << "Shader Compile Error:\n" << &fShaderErrorMsg[0] << endl;
        }
        else
            printf("Compile success\n");

        // 5. Link & use program
        programID = glCreateProgram();
        glAttachShader(programID, vShaderID);
        glAttachShader(programID, fShaderID);
        glLinkProgram(programID);

        glGetProgramiv(programID, GL_LINK_STATUS, &compileResult);
        if(compileResult == GL_FALSE){
            glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
            if (infoLogLength > 0){
                vector<char> ProgramErrorMessage(infoLogLength+1);
                glGetProgramInfoLog(programID, infoLogLength, NULL, &ProgramErrorMessage[0]);
                cout << "\nLink Error: " << &ProgramErrorMessage[0] << endl;
            }
        }

        glDeleteShader(vShaderID);
        glDeleteShader(fShaderID);
    }
    
    // Use the linked program
    void use(){
        glUseProgram(programID);
    }
private:
};

#endif
