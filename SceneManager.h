#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <forward_list>
#include <string>
#include "BoxCollider.h"

struct Scenes
{
	public:
		Scenes() = default;
		Scenes(const std::string& _id) : sceneID(_id) {}
	private:

		std::string sceneID
};

class SceneManager
{
    public:
        void nextScene();
		void loadCurrent();

		void setScenes(const std::forward_list<Scenes>& newScenes);

    private:
        static std::forward_list<Scenes>* scenes;
        SceneManager() = delete;
        
};

#endif
