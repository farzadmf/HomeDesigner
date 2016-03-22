#include "Model.h"

Model::Model() : targetWidget(nullptr) {}

Model::Model(string path, QOpenGLWidget* targetWidget) : targetWidget{ targetWidget }
{
    targetWidget->makeCurrent();
    glGenVertexArrays(1, &boundingBoxVao);
    glGenBuffers(1, &boundingBoxVbo);
    glGenBuffers(1, &boundingBoxEbo);
    targetWidget->doneCurrent();

    LoadModel(path);
    PopulateBoundingBoxVertices();
    PopulateBoundingBoxIndices();
}

void Model::Draw(Shader& shader)
{
    DrawMeshes(shader);
}

void Model::DrawOutline(glm::mat4& model, Shader& shader)
{
    glm::mat4 outlineModel = scale(model, glm::vec3(1.01f));
    glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "model"), 1, GL_FALSE, value_ptr(outlineModel));
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    DrawMeshes(shader);
    // Restore settings for stencil buffer
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
}

void Model::DrawBoundingBox() const
{
    DrawBoundingBox(boundingBoxVertices);
}

void Model::DrawBoundingBox(const vector<glm::vec3>& vertices) const
{
    BufferData(vertices);
    glBindVertexArray(boundingBoxVao);
    glDrawElements(GL_LINES, boundingBoxIndices.size(), GL_UNSIGNED_SHORT, nullptr);
    glBindVertexArray(0);
}

void Model::DrawMeshes(Shader& shader)
{
    for (GLuint i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shader);
}

void Model::PopulateBoundingBoxVertices()
{
    vector<GLfloat> allX, allY, allZ;

    for (auto mesh : meshes)
        for (auto vertex : mesh.Vertices)
        {
            allX.push_back(vertex.Position.x);
            allY.push_back(vertex.Position.y);
            allZ.push_back(vertex.Position.z);
        }

    GLfloat minX, minY, minZ, maxX, maxY, maxZ;
    minX = *min_element(allX.begin(), allX.end());
    maxX = *max_element(allX.begin(), allX.end());
    minY = *min_element(allY.begin(), allY.end());
    maxY = *max_element(allY.begin(), allY.end());
    minZ = *min_element(allZ.begin(), allZ.end());
    maxZ = *max_element(allZ.begin(), allZ.end());

    boundingBoxVertices = vector<glm::vec3>();
    boundingBoxVertices.push_back(glm::vec3(minX, minY, minZ));    // 1
    boundingBoxVertices.push_back(glm::vec3(minX, minY, maxZ));    // 2
    boundingBoxVertices.push_back(glm::vec3(minX, maxY, maxZ));    // 3
    boundingBoxVertices.push_back(glm::vec3(minX, maxY, minZ));    // 4
    boundingBoxVertices.push_back(glm::vec3(maxX, minY, minZ));    // 5
    boundingBoxVertices.push_back(glm::vec3(maxX, maxY, minZ));    // 6
    boundingBoxVertices.push_back(glm::vec3(maxX, maxY, maxZ));    // 7
    boundingBoxVertices.push_back(glm::vec3(maxX, minY, maxZ));    // 8
}

void Model::PopulateBoundingBoxIndices()
{
    boundingBoxIndices = vector<GLushort>();

    // 1 - 2 - 3 - 4
    boundingBoxIndices.push_back(0); boundingBoxIndices.push_back(1);
    boundingBoxIndices.push_back(1); boundingBoxIndices.push_back(2);
    boundingBoxIndices.push_back(2); boundingBoxIndices.push_back(3);
    boundingBoxIndices.push_back(3); boundingBoxIndices.push_back(0);

    // 5 - 6 - 7 - 8
    boundingBoxIndices.push_back(4); boundingBoxIndices.push_back(5);
    boundingBoxIndices.push_back(5); boundingBoxIndices.push_back(6);
    boundingBoxIndices.push_back(6); boundingBoxIndices.push_back(7);
    boundingBoxIndices.push_back(7); boundingBoxIndices.push_back(4);

    // 1 - 2 - 8 - 5
    boundingBoxIndices.push_back(0); boundingBoxIndices.push_back(1);
    boundingBoxIndices.push_back(1); boundingBoxIndices.push_back(7);
    boundingBoxIndices.push_back(7); boundingBoxIndices.push_back(4);
    boundingBoxIndices.push_back(4); boundingBoxIndices.push_back(0);

    // 3 - 4 - 6 - 7
    boundingBoxIndices.push_back(2); boundingBoxIndices.push_back(3);
    boundingBoxIndices.push_back(3); boundingBoxIndices.push_back(5);
    boundingBoxIndices.push_back(5); boundingBoxIndices.push_back(6);
    boundingBoxIndices.push_back(6); boundingBoxIndices.push_back(2);
}

void Model::BufferData(const vector<glm::vec3>& vertices) const
{
    glBindVertexArray(boundingBoxVao);
    glBindBuffer(GL_ARRAY_BUFFER, boundingBoxVbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof vertices[0], &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boundingBoxEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, boundingBoxIndices.size() * sizeof boundingBoxIndices[0], &boundingBoxIndices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid*>(0));
    glBindVertexArray(0);
}

void Model::LoadModel(string path)
{
    Assimp::Importer importer;
    const aiScene* scene =
        importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        cout << "ERROR:ASSIMP::" << importer.GetErrorString() << endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));
    ProcesNode(scene->mRootNode, scene);
}

void Model::ProcesNode(aiNode* node, const aiScene* scene)
{
    for (GLuint i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(mesh, scene));
    }

    for (GLuint i = 0; i < node->mNumChildren; i++)
        ProcesNode(node->mChildren[i], scene);
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) const
{
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<Texture> textures;

    // Process vertices
    for (GLuint i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        // We need this placeholder variable becuase Assimp's data types don't
        //      translate well into GLM
        glm::vec3 positionVector;
        positionVector.x = mesh->mVertices[i].x;
        positionVector.y = mesh->mVertices[i].y;
        positionVector.z = mesh->mVertices[i].z;
        vertex.Position = positionVector;

        glm::vec3 normalVector;
        normalVector.x = mesh->mNormals[i].x;
        normalVector.y = mesh->mNormals[i].y;
        normalVector.z = mesh->mNormals[i].z;
        vertex.Normal = normalVector;

        // For texture coordinates: Assimp allows up to 8 coordinates (we only use 2).
        //      Also, we should check to make sure we indeed have texture coordinates
        if (mesh->mTextureCoords[0])
        {
            glm::vec2 textureVector;
            textureVector.x = mesh->mTextureCoords[0][i].x;
            textureVector.y = mesh->mTextureCoords[0][i].y;
            vertex.TextureCoordinates = textureVector;
        }
        else
            vertex.TextureCoordinates = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    // Process indices
    for (GLuint i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (GLuint j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // Process material
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        vector<Texture> diffuseMaps =
            LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        vector<Texture> specularMaps =
            LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures, targetWidget);
}

vector<Texture> Model::LoadMaterialTextures(aiMaterial* material,
                                            aiTextureType type, string typeName) const
{
    vector<Texture> textures;
    for (GLuint i = 0; i < material->GetTextureCount(type); i++)
    {
        aiString string;
        material->GetTexture(type, i, &string);
        GLboolean skip = false;

        for (GLuint j = 0; i < loadedTextures.size(); j++)
        {
            if (loadedTextures[j].Path == string)
            {
                textures.push_back(loadedTextures[j]);
                skip = true;
                break;
            }
        }

        if (!skip)
        {
            Texture texture;
            texture.Id = TextureFromFile(string.C_Str(), directory);
            texture.Type = typeName;
            texture.Path = string;
            textures.push_back(texture);
        }
    }

    return textures;
}

GLint Model::TextureFromFile(const GLchar* path, string directory) const
{
    targetWidget->makeCurrent();
    string fileName = string(path);
    fileName = directory + '/' + fileName;
    GLuint textureId;
    glGenTextures(1, &textureId);
    int width, height;
    unsigned char* image = SOIL_load_image(fileName.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
    targetWidget->doneCurrent();

    return textureId;
}
