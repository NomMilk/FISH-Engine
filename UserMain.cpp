#include <iostream>
#include "EngineUserHooks.h"

void start(FishEngine::GameInstance *gameInstance)
{
    gameInstance->AddToPhysical(
        BoxCollider(-25, -25, -5, 50, 50, 5)
    );
}

void update(FishEngine::GameInstance *gameInstance, float deltatime)
{
}
