<img width="100" alt="image" src="https://github.com/user-attachments/assets/35a68e73-1dcc-43f3-b9ae-4b201f07c5f6" />

## What is FISH-Engine

**FISH-Engine**  is a minimal, and fast game engine built specifically around **OpenGL 2.1**.

It’s designed for:
- Developers targeting **low-end or older hardware**
- Educational projects focusing on **classic graphics programming**
- Simple indie games and tools with a **lightweight OpenGL core**

## Libraries
GLFW <br>
OpenGL <br>
SFML (For audio primarily) <br>
Assimp (3d model importer) <br>
TinyXML-2 (xml parser to store map data in xml files) <br>
GLM <br>
stb_image <br>
GLAD <br>

To compile goodly, you must have these installed on your system.

## Build Instructions
- Clone the repository
- Download and install dependencies manually
- Build using the provided Makefile
- Run the test project

## Quickstart Guide
- Simply run the editor or runtime through this command in the main directory
```
./bin/editor (for editor)
./bin/runtime (for runtime)
```
- All Engine user code is handled by UserMain.cpp
```
void start(FishEngine::GameInstance *gameInstance)
{
    //runs at the start
}

void update(FishEngine::GameInstance *gameInstance, float deltatime)
{
    //runs every frame
}
```
- Here are some built in commands you can use in the Engine user code!
```
    //adds a physical box collider at position -25, -25, -5 with the scale of 50, 50, 5
    gameInstance->AddToPhysical(
        BoxCollider(-25, -25, -5, 50, 50, 5)
    );

    //teleport player to the position (0, 10, 20)
    gameInstance->TeleportPlayer(0, 10, 20);
```
