#include "scene.hpp"
#include "common/gl_check.hpp"
#include "text/text_draw.hpp"
#include "common/gl_shader.hpp"
#include "core/game_object/objects.hpp"
#include "log/log.hpp"

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

        auto drawCall = std::make_unique<DrawCall>();

        if (gameObject->GetType() == GameObjectType::MODEL) {
            Log("Scene: drawing Model Object");
            drawCall->BindProgramID(LoadShaders("shaders/vertex.glsl", "shaders/fragment.glsl"));
            CheckGLError("Shader Load");
            drawCall->AddLight(glm::vec3(1, 4, 2), 16);
            CheckGLError("Light Add");
            drawCall->SetType(DrawCallType::MESH);
            CheckGLError("DrawCall Type Set");
            drawCall->AddModel("objs/ring.obj");
            CheckGLError("Model Add");
            drawCall->AddTexture("textures/ring.bmp");
            CheckGLError("Texture Add");
            drawCall->BufferInit();
            CheckGLError("Buffer Init");
            glContext->AddDrawCall(std::move(drawCall));
            Log("Scene: Model DrawCall Added");
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
