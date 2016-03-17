#pragma once

#include "Includes.h"
#include "Shader.h"
#include "Mesh.h"
#include <QOpenGLWidget>

class Model
{
public:
    Model();
    Model(string path, QOpenGLWidget* targetWidget);

    void Draw(Shader& shader);
    void DrawOutline(glm::mat4& model, Shader& shader);

    void DrawBoundingBox() const;
    void DrawBoundingBox(const vector<glm::vec3>& vertices) const;

    vector<Texture> GetLoadedTextures() const { return loadedTextures; }
    vector<Mesh> GetMeshes() const { return meshes; }
    vector<glm::vec3> GetBoundingBoxVertices() const { return boundingBoxVertices; }

private:
    // Model data
    vector<Mesh> meshes;
    string directory;
    vector<Texture> loadedTextures;
    vector<glm::vec3> boundingBoxVertices = vector<glm::vec3>();
    vector<GLushort> boundingBoxIndices = vector<GLushort>();
    GLuint boundingBoxVao = 0;
    GLuint boundingBoxVbo = 0;
    GLuint boundingBoxEbo = 0;
    QOpenGLWidget* targetWidget;

    string filePath;

    void DrawMeshes(Shader& shader);
    void PopulateBoundingBoxVertices();
    void PopulateBoundingBoxIndices();
    void BufferData(const vector<glm::vec3>& vertices) const;

    void LoadModel(string path);
    void ProcesNode(aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene) const;

    vector<Texture> LoadMaterialTextures(aiMaterial* material, aiTextureType type,
                                         string typeName) const;
    GLint TextureFromFile(const GLchar* path, string directory) const;
};
