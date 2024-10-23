#include "model_object.hpp"

ModelObject::ModelObject() : GameObject(GameObjectType::MODEL){
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

ModelObject::~ModelObject(){
    
}