#pragma once
#include "game_object.hpp"
#include <string>

class TextObject : public GameObject{
public:
    TextObject();
    ~TextObject();

    void SetPosition(glm::vec2 position);
    void SetRotation(glm::vec1 rotation);
    void SetScale(glm::vec2 scale);
    void SetSize(int size);
    void SetText(std::string text);
    int GetSize();
    std::string GetText();

    glm::vec2 GetPosition();
    glm::vec1 GetRotation();
    glm::vec2 GetScale();



private:
//2d transforms
    glm::vec2 position;
    glm::vec1 rotation;
    glm::vec2 scale;

    int size;
    std::string text;
};