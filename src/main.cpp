#include "WindowManager.h"
#include "components/Camera.h"
#include "systems/RenderSystem.h"
#include "systems/CameraControlSystem.h"
#include "components/Renderable.h"
#include "components/Transform.h"
#include "core/Mediator.h"

#include <chrono>
#include <iostream>
#include <random>

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

    std::vector<Entity> entities(MAX_ENTITIES - 1);

    std::default_random_engine generator;
    std::uniform_real_distribution<float> randPosition(-100.0f, 100.0f);
    std::uniform_real_distribution<float> randRotation(0.0f, 3.0f);
    std::uniform_real_distribution<float> randScale(3.0f, 5.0f);
    std::uniform_real_distribution<float> randColor(0.0f, 1.0f);
    std::uniform_real_distribution<float> randGravity(-10.0f, -1.0f);

    float scale = randScale(generator);

    for (auto& entity : entities)
    {
        entity = gMediator.CreateEntity();


        gMediator.AddComponent<Renderable>(
            entity,
            Renderable{
                .color = glm::vec3(1.0f)
            });

        gMediator.AddComponent<Transform>(
              entity,
              Transform{
                  .position = glm::vec3(0.0f, 0.0f, 0.0f),
                  .rotation = glm::vec3(randRotation(generator), randRotation(generator), randRotation(generator)),
                  .scale = glm::vec3(scale, scale, scale)
              });
    }
    
    float dt = 0.0f;

    while(!quit) {
        auto startTime = std::chrono::high_resolution_clock::now();

        windowManager.ProcessEvents();

        cameraControlSystem->Update(dt);

        renderSystem->Update(dt);

        windowManager.Update();

        auto stopTime = std::chrono::high_resolution_clock::now();

        dt = std::chrono::duration<float, std::chrono::seconds::period>(stopTime - startTime).count();
    }

    windowManager.Shutdown();
    return 0;
}
