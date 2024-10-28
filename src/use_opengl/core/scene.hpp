/*
Scene - Manager of a scene. A scene consists of game objects, lights, cameras, and metadata of the context.
*/

#include <vector>
#include <glm/glm.hpp>
#include <unordered_map>
#include "core/gl_context.hpp"
#include "game_object/game_object.hpp"
#include <memory>

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
        delete gameObject;
    }

    hierarchyNode* AddChild(std::shared_ptr<hierarchyNode> child){
        children.push_back(child.get());
    }

    hierarchyNode* AddChild(std::shared_ptr<GameObject> gameObject){
        children.push_back(new hierarchyNode(gameObject.get()));
        return children.back();
    }
};

class Scene{
public:
    Scene();
    ~Scene();

    void AddGameObject(std::shared_ptr<GameObject> gameObject);
    void RemoveGameObject(const GameObject* gameObject);

    void SetCameraPosition(const glm::vec3& position);
    void SetCameraDirection(const glm::vec3& direction);

    void SetGLContext(const std::shared_ptr<GLContext> glContext);

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
    std::shared_ptr<GLContext> glContext;

    //camera infos
    glm::vec3 cameraPosition;
    glm::vec3 cameraDirection;

    //game objects
    std::unique_ptr<hierarchyNode> hierarchyRoot;
    std::unordered_map<uint32_t, hierarchyNode*> objectMap; //for fast access to gameobject via object id
};