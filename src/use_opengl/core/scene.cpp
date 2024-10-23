#include "scene.hpp"
#include "common/gl_check.hpp"
#include "text/text_draw.hpp"
#include "common/gl_shader.hpp"
#include "core/game_object/objects.hpp"
#include "log/log.hpp"

Scene::Scene(){
    hierarchyRoot = new hierarchyNode();
    cameraPosition = glm::vec3(0.0f, 3.0f, 5.0f);
    cameraDirection = glm::vec3(0.0f, -0.3f, -1.0f);
}

Scene::~Scene(){
    delete hierarchyRoot;
}

void Scene::AddGameObject(GameObject* gameObject){
    if (hierarchyRoot == nullptr){
        hierarchyRoot = new hierarchyNode();
    }

    //Generate an ID for gameObject
    int newId = GenId();
    gameObject->RegisterID(newId);
    objectMap[newId] = hierarchyRoot->AddChild(gameObject);
}

uint32_t Scene::GenId(){
    static uint32_t id = 0;
    return id++;
}

void Scene::Render(){
    glContext->ClearDrawCalls();
    //traverse the scene objects and generate drawcalls for them

    //TO BE OPTIMIZED - current way is adding single drawcall for each object!
    for (auto it = objectMap.begin(); it != objectMap.end(); ++it){
        GameObject* gameObject = it->second->gameObject;

        DrawCall* drawCall = new DrawCall();

        if (gameObject->GetType() == GameObjectType::MODEL){
            drawCall->BindProgramID(LoadShaders("shaders/vertex.glsl", "shaders/fragment.glsl"));
            CheckGLError("Shader Load");
            drawCall->AddLight(glm::vec3(1,4,2), glm::vec3(1,1,1));
            CheckGLError("Light Add");
            drawCall->SetType(DrawCallType::MESH);
            drawCall->AddModel("objs/ring.obj");
            CheckGLError("Model Add");
            drawCall->AddTexture("textures/ring.bmp");
            CheckGLError("Texture Add");
            drawCall->BufferInit();
            CheckGLError("Buffer Init");
            glContext->AddDrawCall(drawCall);
        }else if (gameObject->GetType() == GameObjectType::TEXT){
            //show text
            TextObject* textObject = dynamic_cast<TextObject*>(gameObject);
    
            if (textObject) {
                printText2D((textObject->GetText()).c_str(), 10, 10, 20, glContext);
            } else {
                Error("Failed to cast GameObject to TextObject");
            }
        }
    }

    CheckGLError("Before calling tick() at Scene.Render()");
    
    glContext->Tick(cameraPosition, cameraDirection);
}

void Scene::SetCameraPosition(glm::vec3 position){
    cameraPosition = position;
}

void Scene::SetCameraDirection(glm::vec3 direction){
    cameraDirection = direction;
}

glm::vec3 Scene::GetCameraPosition(){
    return cameraPosition;
}

glm::vec3 Scene::GetCameraDirection(){
    return cameraDirection;
}

void Scene::SetGLContext(GLContext* glContext){
    this->glContext = glContext;
}
