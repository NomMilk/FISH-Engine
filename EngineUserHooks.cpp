#include <vector>
#include <algorithm>
#include "EngineUserHooks.h"
#include "BoxCollider.h"

namespace FishEngine
{
	void GameInstance::AddToPhysical(BoxCollider _collider)
	{
		pushbackColliders.push_back(_collider);
	}

	void GameInstance::TeleportPlayer(float x, float y, float z)
	{
		player.Position = glm::vec3(x, z, y);
	}

	void GameInstance::RemoveFromPhysical(BoxCollider _collider)
	{

	}
}

