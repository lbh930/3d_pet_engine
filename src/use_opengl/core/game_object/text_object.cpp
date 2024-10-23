#include "text_object.hpp"

TextObject::TextObject() : GameObject(GameObjectType::TEXT){
    position = glm::vec2(0.0f, 0.0f);
    rotation = glm::vec1(0.0f);
    scale = glm::vec2(1.0f, 1.0f);
    size = 20;
    text = "Hello World";
}

TextObject::~TextObject(){
    
}

void TextObject::SetPosition(glm::vec2 position){
    this->position = position;
}

void TextObject::SetRotation(glm::vec1 rotation){
    this->rotation = rotation;
}

void TextObject::SetScale(glm::vec2 scale){
    this->scale = scale;
}

glm::vec2 TextObject::GetPosition(){
    return position;
}

glm::vec1 TextObject::GetRotation(){
    return rotation;
}

glm::vec2 TextObject::GetScale(){
    return scale;
}

int TextObject::GetSize(){
    return size;
}

std::string TextObject::GetText(){
    return text;
}

void TextObject::SetSize(int size){
    this->size = size;
}

void TextObject::SetText(std::string text){
    this->text = text;
}
