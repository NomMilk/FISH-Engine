#pragma once

#include <forward_list>
#include <string>
#include "BoxCollider.h"
#include "modelLoader.h"

struct Scenes
{
	public:
		Scenes(const std::string& _id, BoxCollider& _enterCollider, BoxCollider& _exitCollider)
		: sceneID(_id), enterCollider(_enterCollider), exitCollider(_exitCollider) {}

	private:
		std::string sceneID;

		BoxCollider exitCollider;
		BoxCollider enterCollider;
		glm::vec3 teleportPoint;

};

class SceneManager
{
    public:
        void nextScene();
		void prevScene();

		void setScenes(const std::forward_list<Scenes>& newScenes);

    private:
        std::forward_list<Scenes>* scenes;
		ModelLoader modelLoader;
        
};
