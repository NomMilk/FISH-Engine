#ifndef SCENELOADER_H 

#include <string>
#include <vector>

/*
here's the plan, the scene loader will load these scene objects
every scene has three part, the trigger to load the next scene and
previous scene and the colliders of the current scene
the trigger will load the next scene when you entered so we can have big maps
hopefully ;-;

actually I odn't know if this is needed but i'll just delete it if we dont.

i'll have to talk to travis about the implentation to link this to the model loader and to see
in the first place if it's a good idea. 
*/
struct Scene{
	public:
		Scene(BoxCollider nextSceneCollider, std::vector<BoxCollider> currentColliders, BoxCollider preSceneCollider)
			:nextSceneCollider(nextSceneCollider),
			 currentColliders(currentColliders),
			 preSceneCollider(preSceneCollider)

		BoxCollider nextSceneCollider;
		std::vector<BoxCollider> currentColliders;
		BoxCollider preSceneCollider;
};

class SceneLoader
{
    public:
		std::vector<Scene> stages;
};

#endif
