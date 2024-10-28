#include "game_object.hpp"

GameObject::GameObject(GameObjectType type){
    this->type = type;
}

GameObject::~GameObject(){
    
}

void GameObject::RegisterID(int id){
    this->id = id;
}

GameObjectType GameObject::GetType(){
    return type;
}

std::shared_ptr<DrawCall> GameObject::GetDrawCall(){
    return drawCall;
}

void GameObject::SetDrawCall(std::shared_ptr<DrawCall> drawCall){
    this->drawCall = drawCall;
}