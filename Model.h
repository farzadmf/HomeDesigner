#pragma once

#include "Includes.h"
#include "Shader.h"
#include "Mesh.h"
#include <QOpenGLWidget>
///
/// \brief The Model class
///  Which does the drawing of the model and the outline bounding box and which contain multiple textures and meshes
///
class Model
{
public:
    Model();
    ///
    /// \brief Model Loads and populates the model
    /// \param targetWidget the target widget
    /// \param path the path to the file
    ///
    Model(QOpenGLWidget* targetWidget, string path);
    ///
    /// \brief Draw loops over the meshes to draw them
    /// \param shader
    ///
    void Draw(Shader& shader);
    ///
    /// \brief DrawOutline draws for each a slightly large outline of the model
    /// \param model
    /// \param shader
    ///
    void DrawOutline(glm::mat4& model, Shader& shader);
    ///
    /// \brief DrawBoundingBox draws the bounding box will provide
    ///
    void DrawBoundingBox() const;
    ///
    /// \brief DrawBoundingBox draws the bounding box by taking as input the vertices
    /// \param vertices the vertices for drawing the bounding box
    ///
    void DrawBoundingBox(const vector<glm::vec3>& vertices) const;

    vector<Texture> GetLoadedTextures() const { return loadedTextures; }
    vector<Mesh> GetMeshes() const { return meshes; }
    vector<glm::vec3> GetBoundingBoxVertices() const { return boundingBoxVertices; }

    // Getter for model file path
    string GetFilePath() const { return filePath; }

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
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

    vector<Texture> LoadMaterialTextures(aiMaterial* material, aiTextureType type,
                                         string typeName);
    GLuint TextureFromFile(const GLchar* path, string directory) const;
};
