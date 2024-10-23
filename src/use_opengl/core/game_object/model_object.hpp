#pragma once
#include "game_object.hpp"
class ModelObject : public GameObject{
public:
    ModelObject();
    ~ModelObject();

    void SetPosition(glm::vec3 position);
    void SetRotation(glm::vec3 rotation);
    void SetScale(glm::vec3 scale);

    glm::vec3 GetPosition();
    glm::vec3 GetRotation();
    glm::vec3 GetScale();
private:
    //3d transforms
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};