#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <forward_list>

struct Scenes
{
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
