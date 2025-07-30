#include "modelLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <tinyxml2.h>

using namespace std;

ModelLoader::ModelLoader() {
}

ModelLoader::~ModelLoader() {
    cleanup();
}

bool ModelLoader::loadFromXML(const string& xmlPath) {
    cleanup();
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError result = doc.LoadFile(xmlPath.c_str());
    if (result != tinyxml2::XML_SUCCESS) {
        cout << "Couldn't open XML file: " << xmlPath << endl;
        cout << "Error code: " << result << endl;
        return false;
    }
    tinyxml2::XMLElement* root = doc.RootElement();
    if (!root) {
        cout << "XML file doesn't have a root element!" << endl;
        return false;
    }
    tinyxml2::XMLElement* modelsElement = root->FirstChildElement("models");
    if (!modelsElement) {
        cout << "No models found in XML!" << endl;
        return false;
    }
    tinyxml2::XMLElement* modelElement = modelsElement->FirstChildElement("model");
    while (modelElement) {
        ModelData currentModel;
        currentModel.position = glm::vec3(0.0f);
        currentModel.rotation = glm::vec3(0.0f);
        currentModel.scale = glm::vec3(1.0f);
        tinyxml2::XMLElement* pathElement = modelElement->FirstChildElement("path");
        if (pathElement && pathElement->GetText()) {
            currentModel.path = pathElement->GetText();
        }
        tinyxml2::XMLElement* posElement = modelElement->FirstChildElement("position");
        if (posElement) {
            currentModel.position = parseVectorElement(posElement);
        }
        tinyxml2::XMLElement* rotElement = modelElement->FirstChildElement("rotation");
        if (rotElement) {
            currentModel.rotation = parseVectorElement(rotElement);
        }
        tinyxml2::XMLElement* scaleElement = modelElement->FirstChildElement("scale");
        if (scaleElement) {
            currentModel.scale = parseVectorElement(scaleElement);
        }
        if (!currentModel.path.empty()) {
            try {
                currentModel.model = new Model(currentModel.path.c_str());
                models.push_back(currentModel);
                cout << "Loaded model: " << currentModel.path << endl;
            } catch (const exception& e) {
                cout << "Failed to load model: " << currentModel.path << " - " << e.what() << endl;
            }
        }
        modelElement = modelElement->NextSiblingElement("model");
    }
    return !models.empty();
}

void ModelLoader::drawModels(Shader& shader) {
    for (int i = 0; i < models.size(); i++) {
        if (models[i].model) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, models[i].position);
            model = glm::rotate(model, glm::radians(models[i].rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(models[i].rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(models[i].rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, models[i].scale);
            glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
            
            // Calculate and set normal matrix for lighting calculations
            glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
            glUniformMatrix3fv(glGetUniformLocation(shader.ID, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
            
            try {
                models[i].model->Draw(shader);
            } catch (const exception& e) {
                cout << "Error drawing model: " << e.what() << endl;
            }
        }
    }
}

glm::vec3 ModelLoader::parseVectorElement(tinyxml2::XMLElement* element) {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    
    if (element) {
        element->QueryFloatAttribute("x", &x);
        element->QueryFloatAttribute("y", &y);
        element->QueryFloatAttribute("z", &z);
    }
    return glm::vec3(x, y, z);
}

size_t ModelLoader::getModelCount() const {
    return models.size();
}

const ModelData* ModelLoader::getModelData(size_t index) const {
    if (index < models.size()) {
        return &models[index];
    }
    return nullptr;
}

void ModelLoader::cleanup() {
    for (int i = 0; i < models.size(); i++) {
        if (models[i].model) {
            delete models[i].model;
            models[i].model = nullptr;
        }
    }
    models.clear();
} 