/*
Scene - Manager of a scene. A scene consists of game objects, lights, cameras, and metadata of the context.
*/

#include <vector>
#include <glm/glm.hpp>
#include <unordered_map>
#include "core/gl_context.hpp"
#include "game_object/game_object.hpp"

//a scene is structured as a tree
struct hierarchyNode{
    hierarchyNode* parent;
    GameObject* gameObject;
    std::vector<hierarchyNode*> children;

    hierarchyNode(hierarchyNode* parent): parent(parent){}
    hierarchyNode(GameObject* gameObject): parent(nullptr), gameObject(gameObject){}

    hierarchyNode(): parent(nullptr){}
    ~hierarchyNode(){
        for (auto child : children){
            delete child;
        }
    }

    hierarchyNode* AddChild(hierarchyNode* child){
        children.push_back(child);
    }

    hierarchyNode* AddChild(GameObject* gameObject){
        children.push_back(new hierarchyNode(gameObject));
        return children.back();
    }
};

class Scene{
public:
    Scene();
    ~Scene();

    void AddGameObject(GameObject* gameObject);
    void RemoveGameObject(GameObject* gameObject);

    void SetCameraPosition(glm::vec3 position);
    void SetCameraDirection(glm::vec3 direction);

    void SetGLContext(GLContext* glContext);

    glm::vec3 GetCameraPosition();
    glm::vec3 GetCameraDirection();

    //utils
    uint32_t GenId();

    void Update();
    void Render();

    void SetRoot(GameObject* root);
    GameObject* GetRoot();

private:
    //Scene contexts
    GLContext* glContext;

    //camera infos
    glm::vec3 cameraPosition;
    glm::vec3 cameraDirection;

    //game objects
    hierarchyNode* hierarchyRoot;
    std::unordered_map<uint32_t, hierarchyNode*> objectMap; //for fast access to gameobject via object id
};