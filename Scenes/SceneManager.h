#define SCENEMANAGER_H

struct Scenes
{
}

class SceneManager
{
    public:
        public static void nextScene();

    private:
        std::forward_list<Scenes> scenes;
        SceneManager() = delete;
        
};

#endif
