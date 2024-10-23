/*
GameObject, Parent of all visualizable in the PetEngine3D
I remember the good old days when I was playing with Unity Engine for my projects.
    - Benhao "LaoLu" Lu
*/
#pragma once

#include <glm/glm.hpp>

enum class GameObjectType{
    // The GameObject is a 3D model
    MODEL,
    // The GameObject is a 2D sprite
    SPRITE,
    // The GameObject is a 2D text
    TEXT,
    //
    EMPTY
};

class GameObject{
public:
    GameObject(GameObjectType type);
    void RegisterID(int id);
    virtual ~GameObject();
    GameObjectType GetType();

private:
    uint32_t id = -1;
    GameObjectType type;
};