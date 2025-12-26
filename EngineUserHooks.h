#pragma once
#include <vector>
#include "BoxCollider.h"
#include "Camera.h"

namespace FishEngine
{
	class GameInstance 
	{
		public:
			GameInstance(Camera& _player)
			: player(_player) {}

			void AddToPhysical(BoxCollider _collider);
			void RemoveFromPhysical(BoxCollider _collider);

			void TeleportPlayer(float x, float y, float z);

			const std::vector<BoxCollider> Getpushback() const {return pushbackColliders; }

		private:
			Camera& player;
			std::vector<BoxCollider> pushbackColliders;
	};
}

