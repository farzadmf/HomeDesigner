#pragma once

#include "Includes.h"

bool checkShaderCompile(GLuint shader);
void errorCallBack(int error, const char* description);

//const GLuint WIDTH = 800, HEIGHT = 600;

inline void ex()
{
    cout << endl << "--------------------------------------------------" << endl;
    cout << "Press Enter to continue ..." << endl;
    getchar();
    exit(0);
}

inline void errorCallBack(int error, const char* description)
{
    cout << "========================================================" << endl;
    cout << "ERROR: '" << error << "', DESCRIPTION: " << endl;
    cout << description << endl;
    cout << "========================================================" << endl;
}

inline GLuint generateVertexShader(const GLchar* source)
{
    GLuint shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    cout << "Checking vertex shader '" << shader << "' compile:" << endl;
    bool success = checkShaderCompile(shader);
    cout << "-----------------------------------------------" << endl;

    return success ? shader : 0;
}

inline GLuint GenerateGeometryShader(const GLchar* source)
{
    GLuint shader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    cout << "Checking geometry shader '" << shader << "' compile:" << endl;
    bool success = checkShaderCompile(shader);
    cout << "-----------------------------------------------" << endl;

    return success ? shader : 0;
}

inline GLuint generateFragmentShader(const GLchar* source)
{
    GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);

    cout << "Checking fragment shader '" << shader << "' compile:" << endl;
    bool success = checkShaderCompile(shader);
    cout << "-----------------------------------------------" << endl;

    return success ? shader : 0;
}

inline GLuint GenerateShader(GLenum type, const GLchar* filePath)
{
    ifstream sourceFile;
    sourceFile.open(filePath);
    if (sourceFile.fail())
        return 0;

    // From here: http://www.gamedev.net/topic/353162-reading-a-whole-file-into-a-string-with-ifstream/
    string sourceString(
        (std::istreambuf_iterator<char>(sourceFile)),
        std::istreambuf_iterator<char>()
    );

    const GLchar * source = sourceString.c_str();
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    cout << "Checking shader compile:" << endl;
    checkShaderCompile(shader);
    cout << "-----------------------------------------------" << endl;

    return shader;
}

inline bool checkShaderCompile(GLuint shader)
{
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status != GL_TRUE)
    {
        char buffer[512];
        glGetShaderInfoLog(shader, 512, nullptr, buffer);
        cout << buffer << endl;
    }
    else
        cout << "COMPILE OK!" << endl;

    return status;
}

inline bool CheckProgramLink(GLuint program)
{
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);

    if (status != GL_TRUE)
    {
        char buffer[512];
        glGetProgramInfoLog(program, 512, nullptr, buffer);
        cout << buffer << endl;
    }
    else
        cout << "LINK OK!" << endl;

    return status;
}

inline GLuint GenerateShaderProgram(GLuint vertexShader, GLuint fragmentShader,
                                  GLuint geometryShader)
{
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glAttachShader(program, geometryShader);
    glLinkProgram(program);

    cout << "Checking shader program link:" << endl;
    bool success = CheckProgramLink(program);
    cout << "-----------------------------------------------" << endl;

    return success ? program : 0;
}

inline GLuint GenerateShaderProgram(GLuint vertexShader, GLuint fragmentShader)
{
    return GenerateShaderProgram(vertexShader, fragmentShader, 0);
}

inline void loadTexture(GLuint texture, char* texturePath)
{
    int width, height;
    GLubyte* image;

    glBindTexture(GL_TEXTURE_2D, texture);
    image = SOIL_load_image(texturePath, &width, &height, nullptr, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
}

inline GLuint loadTexture(char* texturePath)
{
    GLuint textureId;
    glGenTextures(1, &textureId);

    int width, height;
    GLubyte* image;

    image = SOIL_load_image(texturePath, &width, &height, nullptr, SOIL_LOAD_RGB);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
    return textureId;
}

inline GLuint loadTextureWithAlpha(char* texturePath)
{
    GLuint textureId;
    glGenTextures(1, &textureId);

    int width, height;
    GLubyte* image;

    image = SOIL_load_image(texturePath, &width, &height, nullptr, SOIL_LOAD_RGBA);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
    return textureId;
}

inline GLuint generateAttachmentTexture(GLuint screenWidth, GLuint screenHeight,
                                        GLboolean depth, GLboolean stencil)
{
    GLenum attachmentType = 0;
    if (!depth && !stencil)
        attachmentType = GL_RGB;
    else if (depth && !stencil)
        attachmentType = GL_DEPTH_COMPONENT;
    else if (!depth && stencil)
        attachmentType = GL_STENCIL_INDEX;

    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    if (!depth && !stencil)
        glTexImage2D(GL_TEXTURE_2D, 0, attachmentType, screenWidth, screenHeight,
                     0, attachmentType, GL_UNSIGNED_BYTE, nullptr);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, screenWidth, screenHeight,
                     0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    return textureId;
}

inline GLuint LoadCubeMap(vector<const GLchar*> faces)
{
    GLuint textureId;
    glGenTextures(1, &textureId);
//    glActiveTexture(GL_TEXTURE0);

    int width, height;
    GLubyte* image;

    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
    for (GLuint i = 0; i < faces.size(); i++)
    {
        image = SOIL_load_image(faces[i], &width, &height, nullptr, SOIL_LOAD_RGB);
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
            GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        SOIL_free_image_data(image);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return textureId;
}
