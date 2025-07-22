#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glad/glad.h>
#include "shaderClass.h"
#include "Texture.h"

struct Mesh {
    // OpenGL 2.1 doesn't use VAOs, so we just store VBO and EBO
    unsigned int VBO, EBO;
    unsigned int indexCount;
    unsigned int materialIndex;
};

class Model {
    public:
        Model(const char* path);
        ~Model();
        void Draw(Shader& shader);

    private:
        std::vector<Mesh> meshes;
        std::vector<Texture*> textures;
        std::vector<Texture*> loadedTextures;
        std::string directory;
        
        void loadModel(const std::string& path);
        void processMesh(aiMesh* mesh, const aiScene* scene);
        void loadMaterialTextures(aiMaterial* mat, aiTextureType type, unsigned int materialIndex);
};