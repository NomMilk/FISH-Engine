#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "Model.h"
#include <tinyxml2.h>

struct ModelData {
    std::string name;
    std::string path;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    Model* model;
};

class ModelLoader {
public:
    ModelLoader();
    ~ModelLoader();
    bool loadFromXML(const std::string& xmlPath);
    bool saveToXML(const std::string& xmlPath);
    void drawModels(Shader& shader);
    void drawColliders(Shader& shader);
    size_t getModelCount() const;
    const ModelData* getModelData(size_t index) const;
    ModelData* getModelData(size_t index);
    void addModel(const std::string& name);
	bool alreadyExist(const std::string& name) const;
    void loadModelForIndex(size_t index);
private:
    std::vector<ModelData> models;
    glm::vec3 parseVectorElement(tinyxml2::XMLElement* element);
    void cleanup();
};

#endif // MODEL_LOADER_H 
