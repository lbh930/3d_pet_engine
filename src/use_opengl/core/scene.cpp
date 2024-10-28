#include "scene.hpp"
#include "common/gl_check.hpp"
#include "text/text_draw.hpp"
#include "common/bmp_loader.hpp"
#include "common/obj_loader.hpp"
#include "common/gl_shader.hpp"
#include "core/game_object/objects.hpp"
#include "log/log.hpp"
#include <thread>

Scene::Scene()
    : hierarchyRoot(std::make_unique<hierarchyNode>()),
      cameraPosition(0.0f, 3.0f, 5.0f),
      cameraDirection(0.0f, -0.3f, -1.0f)
{
}

Scene::~Scene()
{
    // Smart pointers automatically clean up
}

void Scene::AddGameObject(std::shared_ptr<GameObject> gameObject)
{
    if (!hierarchyRoot) {
        hierarchyRoot = std::make_unique<hierarchyNode>();
    }

    // Generate an ID for gameObject
    uint32_t newId = GenId();
    gameObject->RegisterID(newId);
    objectMap[newId] = hierarchyRoot->AddChild(gameObject);
}

uint32_t Scene::GenId()
{
    static uint32_t id = 0;
    return id++;
}

void Scene::Render()
{
    Log("Scene Render");
    glContext->ClearDrawCalls();
    // Traverse the scene objects and generate draw calls for them

    // TODO: Optimize - current way is adding single draw call for each object
    for (auto& [id, node] : objectMap) {
        GameObject* gameObject = node->gameObject;

        if (gameObject->GetType() == GameObjectType::MODEL) {
            auto drawCall = gameObject->GetDrawCall();
            if (drawCall == nullptr) {
                std::vector<std::thread> threads; //temp thread pool for resource load

                drawCall = std::make_shared<DrawCall>();
                std::vector<unsigned char> textureData;
                std::pair <int, int> textureSize;
                Log("Scene: drawing Model Object");

                GLuint programID = 0;

                //threads.push_back(std::thread([drawCall, &programID](){
                    programID = LoadShaders("shaders/vertex.glsl", "shaders/fragment.glsl");
                    Log("Scene: Object Shader Loaded, ID: ", programID);
               //}));

                //threads.push_back(std::thread([drawCall](){
                    drawCall->AddModel("objs/ring.obj");
                //}));

                threads.push_back(std::thread([drawCall, &textureData, &textureSize](){
                    loadBMP("textures/ring.bmp", textureData, textureSize);
                }));

                for (auto& thread : threads){
                    thread.join();
                }

                CheckGLError("Resource (Model & Texture & Shader) Add");

                drawCall->BindProgramID(programID);
                drawCall->AddTexture(textureData, textureSize);
                drawCall->AddLight(glm::vec3(1, 4, 2), 16);
                CheckGLError("Light Add");
                drawCall->SetType(DrawCallType::MESH);
                CheckGLError("DrawCall Type Set");

                //do buffer init only after all resources are loaded
                drawCall->BufferInit();
                CheckGLError("Buffer Init");
                glContext->AddDrawCall(drawCall);
                Log("Scene: Model DrawCall Added");
                gameObject->SetDrawCall(drawCall);
            }else{
                Log("Scene: Model DrawCall already exists");
                glContext->AddDrawCall(drawCall);
            }
        } else if (gameObject->GetType() == GameObjectType::TEXT) {
            Log("Scene: drawing Text Object");
            // Show text
            TextObject* textObject = dynamic_cast<TextObject*>(gameObject);

            if (textObject) {
                printText2D(std::string_view(textObject->GetText()), 10, 10, 20, glContext.get());
            } else {
                Error("Failed to cast GameObject to TextObject");
            }
            Log("Scene: Text Drawn");
        }
    }

    CheckGLError("Before calling tick() at Scene.Render()");

    glContext->Tick(cameraPosition, cameraDirection);
}

void Scene::SetCameraPosition(const glm::vec3& position)
{
    cameraPosition = position;
}

void Scene::SetCameraDirection(const glm::vec3& direction)
{
    cameraDirection = direction;
}

glm::vec3 Scene::GetCameraPosition()
{
    return cameraPosition;
}

glm::vec3 Scene::GetCameraDirection()
{
    return cameraDirection;
}

void Scene::SetGLContext(std::shared_ptr<GLContext> glContext)
{
    this->glContext = glContext;
}
