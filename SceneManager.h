#pragma once

#include <forward_list>
#include <string>
#include "BoxCollider.h"
#include "modelLoader.h"

struct Scenes
{
	public:
		Scenes(const std::string& _id, BoxCollider& _enterCollider, BoxCollider& _exitCollider)
		: sceneID(_id) {}

	private:
		std::string sceneID;
		glm::vec3 teleportPoint;
};

class SceneManager
{
    public:
		SceneManager(const ModelLoader _ModelLoader) : modelLoader(_ModelLoader) {}
        void nextScene();
		void prevScene();

		void setScenes(const std::forward_list<Scenes>& newScenes);
		void addScene(Scenes scene);

    private:
		Scenes* selectedScene;
        std::forward_list<Scenes>* scenes;
		ModelLoader modelLoader;
        
};
