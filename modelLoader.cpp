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
        currentModel.name = "";
        currentModel.position = glm::vec3(0.0f);
        currentModel.rotation = glm::vec3(0.0f);
        currentModel.scale = glm::vec3(1.0f);
        const char* nameAttr = modelElement->Attribute("name");
        if (nameAttr) {
            currentModel.name = nameAttr;
        }
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

void ModelLoader::drawColliders(Shader& shader) {
    for (int i = 0; i < models.size(); i++) {
        if (models[i].model) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, models[i].position);
            model = glm::rotate(model, glm::radians(models[i].rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(models[i].rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(models[i].rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, models[i].scale);
            glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
            
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

ModelData* ModelLoader::getModelData(size_t index) {
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

void ModelLoader::addModel(const std::string& name) {
    ModelData newModel;
    newModel.name = name;
    newModel.path = "";
    newModel.position = glm::vec3(0.0f, 0.0f, 0.0f);
    newModel.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    newModel.scale = glm::vec3(1.0f, 1.0f, 1.0f);
    newModel.model = nullptr;
    models.push_back(newModel);
}

bool ModelLoader::alreadyExist(const std::string& name) const {
	for (size_t i = 0; i < models.size(); ++i) {
		if(models[i].name == name) return true;
	}
	return false;
}

void ModelLoader::loadModelForIndex(size_t index) {
    if (index >= models.size()) {
        return;
    }
    if (models[index].model != nullptr) {
        delete models[index].model;
        models[index].model = nullptr;
    }
    if (!models[index].path.empty()) {
        try {
            models[index].model = new Model(models[index].path.c_str());
            cout << "Loaded model: " << models[index].path << endl;
        } catch (const exception& e) {
            cout << "Failed to load model: " << models[index].path << " - " << e.what() << endl;
            models[index].model = nullptr;
        }
    }
}

bool ModelLoader::saveToXML(const std::string& xmlPath) {
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLDeclaration* decl = doc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
    doc.InsertFirstChild(decl);
    
    tinyxml2::XMLElement* root = doc.NewElement("scene");
    doc.InsertEndChild(root);
    
    tinyxml2::XMLElement* modelsElement = doc.NewElement("models");
    root->InsertEndChild(modelsElement);
    
    for (size_t i = 0; i < models.size(); i++) {
        tinyxml2::XMLElement* modelElement = doc.NewElement("model");
        if (!models[i].name.empty()) {
            modelElement->SetAttribute("name", models[i].name.c_str());
        }
        
        tinyxml2::XMLElement* pathElement = doc.NewElement("path");
        tinyxml2::XMLText* pathText = doc.NewText(models[i].path.c_str());
        pathElement->InsertEndChild(pathText);
        modelElement->InsertEndChild(pathElement);
        
        tinyxml2::XMLElement* posElement = doc.NewElement("position");
        posElement->SetAttribute("x", models[i].position.x);
        posElement->SetAttribute("y", models[i].position.y);
        posElement->SetAttribute("z", models[i].position.z);
        modelElement->InsertEndChild(posElement);
        
        tinyxml2::XMLElement* rotElement = doc.NewElement("rotation");
        rotElement->SetAttribute("x", models[i].rotation.x);
        rotElement->SetAttribute("y", models[i].rotation.y);
        rotElement->SetAttribute("z", models[i].rotation.z);
        modelElement->InsertEndChild(rotElement);
        
        tinyxml2::XMLElement* scaleElement = doc.NewElement("scale");
        scaleElement->SetAttribute("x", models[i].scale.x);
        scaleElement->SetAttribute("y", models[i].scale.y);
        scaleElement->SetAttribute("z", models[i].scale.z);
        modelElement->InsertEndChild(scaleElement);
        
        modelsElement->InsertEndChild(modelElement);
    }
    
    tinyxml2::XMLError result = doc.SaveFile(xmlPath.c_str());
    if (result != tinyxml2::XML_SUCCESS) {
        cout << "Failed to save XML file: " << xmlPath << endl;
        return false;
    }
    cout << "Saved " << models.size() << " models to " << xmlPath << endl;
    return true;
} 
