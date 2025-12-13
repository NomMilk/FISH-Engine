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

	void GameInstance::RemoveFromPhysical(BoxCollider _collider)
	{

	}
}

