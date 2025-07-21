#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glad/glad.h>
#include "shaderClass.h"

struct Mesh {
    unsigned int VAO, VBO, EBO;
    unsigned int indexCount;
};

class Model {
    public:
        Model(const char* path);
        ~Model();
        void Draw();

    private:
        std::vector<Mesh> meshes;
        
        void loadModel(const std::string& path);
        void processMesh(aiMesh* mesh, const aiScene* scene);
};