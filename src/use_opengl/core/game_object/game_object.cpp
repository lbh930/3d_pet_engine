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