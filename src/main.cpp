#include "WindowManager.h"
#include "components/Camera.h"
#include "systems/RenderSystem.h"
#include "systems/CameraControlSystem.h"
#include "components/Renderable.h"
#include "components/Transform.h"
#include "core/Mediator.h"
#include "graphics/PrimitiveMeshes.h"

#include <chrono>
#include <iostream>
#include <random>

#include "components/Cubemap.h"
#include "systems/SkyboxRenderSystem.h"

Mediator gMediator;

bool quit = false;

void QuitHandler(Event& event)
{
    quit = true;
}

int main() {
    gMediator.Init();

    WindowManager windowManager;
    windowManager.Init("Hello World", 1920, 1080, 0, 0);

    gMediator.AddEventListener(FUNCTION_LISTENER(Events::Window::QUIT, QuitHandler));

    gMediator.RegisterComponent<Camera>();
    gMediator.RegisterComponent<Cubemap>();
    gMediator.RegisterComponent<Renderable>();
    gMediator.RegisterComponent<Transform>();



    auto cameraControlSystem = gMediator.RegisterSystem<CameraControlSystem>();
    {
        Signature signature;
        signature.set(gMediator.GetComponentType<Camera>());
        signature.set(gMediator.GetComponentType<Transform>());
        gMediator.SetSystemSignature<CameraControlSystem>(signature);
    }

    cameraControlSystem->Init();


    auto renderSystem = gMediator.RegisterSystem<RenderSystem>();
    {
        Signature signature;
        signature.set(gMediator.GetComponentType<Renderable>());
        signature.set(gMediator.GetComponentType<Transform>());
        gMediator.SetSystemSignature<RenderSystem>(signature);
    }

    renderSystem->Init();

    auto skyboxRenderSystem = gMediator.RegisterSystem<SkyboxRenderSystem>();
    {
        Signature signature;
        signature.set(gMediator.GetComponentType<Cubemap>());
        gMediator.SetSystemSignature<SkyboxRenderSystem>(signature);
    }


    skyboxRenderSystem->Init();


    std::vector<Entity> entities(MAX_ENTITIES - 1);

    std::default_random_engine generator;
    std::uniform_real_distribution<float> randPosition(-10.0f, 10.0f);
    std::uniform_real_distribution<float> randRotation(0.0f, 3.0f);
    std::uniform_real_distribution<float> randScale(3.0f, 5.0f);
    std::uniform_real_distribution<float> randColor(0.0f, 1.0f);
    std::uniform_real_distribution<float> randGravity(-10.0f, -1.0f);

    float scale = randScale(generator);

    // init all entities empty
    for (auto& entity : entities)
    {
        entity = gMediator.CreateEntity();
    }


    // Sphere
    gMediator.AddComponent<Renderable>(
        entities[0],
        Renderable{
            .model = std::make_shared<Model>(createSphereMesh()),
            .color = glm::vec3(0.9f, 0.15f, 0.1f)
        });

    gMediator.AddComponent<Transform>(
          entities[0],
          Transform{
              .position = glm::vec3(6.0f, 7.0f, -3.0f),
              .rotation = glm::vec3(randRotation(generator), randRotation(generator), randRotation(generator)),
              .scale = glm::vec3(1.0f)
          });

    // Table
    gMediator.AddComponent<Renderable>(
        entities[1],
        Renderable{
            .model = std::make_shared<Model>(createTableMesh()),
            .color = glm::vec3(0.45f, 0.3f, 0.2f)
        });

    gMediator.AddComponent<Transform>(
          entities[1],
          Transform{
              .position = glm::vec3(0.0f),
              .rotation = glm::vec3(0.0f),
              .scale = glm::vec3(5.0f)
          });

    // Lamp
    gMediator.AddComponent<Renderable>(
        entities[2],
        Renderable{
            .model = std::make_shared<Model>(createDeskLampMesh()),
            .color = glm::vec3(0.7f, 0.7f, 0.75f)
        });

    gMediator.AddComponent<Transform>(
          entities[2],
          Transform{
              .position = glm::vec3(4.0f, 6.0f, 0.0f),
              .rotation = glm::vec3(0.0f),
              .scale = glm::vec3(1.0f)
          });


    // Skybox
    {
        std::vector<std::string> cubemapPaths = {
            "resources/textures/skybox/right.jpg",
            "resources/textures/skybox/left.jpg",
            "resources/textures/skybox/top.jpg",
            "resources/textures/skybox/bottom.jpg",
            "resources/textures/skybox/front.jpg",
            "resources/textures/skybox/back.jpg"
        };

        gMediator.AddComponent<Cubemap>(
            entities[3],
            Cubemap{
                .skybox = std::make_shared<Skybox>(cubemapPaths)
            });
    }

    // Ground
    gMediator.AddComponent<Renderable>(
    entities[4],
    Renderable{
        .model = std::make_shared<Model>(createGroundMesh("resources/textures/skybox/right.jpg")),
        .color = glm::vec3(randColor(generator), randColor(generator), randColor(generator))
    });

    gMediator.AddComponent<Transform>(
          entities[4],
          Transform{
              .position = glm::vec3(0.0f),
              .rotation = glm::vec3(0.0f),
              .scale = glm::vec3(1.0f)
          });


    gMediator.AddComponent<Renderable>(
        entities[5],
        Renderable{
            .model = std::make_shared<Model>("resources/models/dragon/scene.gltf"),
            .color = glm::vec3(randColor(generator), randColor(generator), randColor(generator))
        });

    gMediator.AddComponent<Transform>(
          entities[5],
          Transform{
              .position = glm::vec3(-3.0f, 2.0f, -5.0f),
              .rotation = glm::vec3(0.0f),
              .scale = glm::vec3(0.06f)
          });

    // Delta time
    float dt = 0.0f;

    while(!quit) {
        auto startTime = std::chrono::high_resolution_clock::now();

        windowManager.Update();

        windowManager.ProcessEvents();

        skyboxRenderSystem->Update(dt);

        cameraControlSystem->Update(dt);

        renderSystem->Update(dt);

        auto stopTime = std::chrono::high_resolution_clock::now();

        dt = std::chrono::duration<float, std::chrono::seconds::period>(stopTime - startTime).count();
    }

    windowManager.Shutdown();
    return 0;
}
