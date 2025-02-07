#include "RenderSystem.h"

#include "components/Camera.h"
#include "components/Renderable.h"
#include "components/Transform.h"
#include "core/Mediator.h"
#include "graphics/Shader.h"
#include <cmath>


extern Mediator gMediator;


void RenderSystem::Init()
{
	gMediator.AddEventListener(METHOD_LISTENER(Events::Window::RESIZED, RenderSystem::WindowSizeListener));

	shader = std::make_unique<Shader>("resources/shaders/simple.vert", "resources/Shaders/simple.frag");

	mCamera = gMediator.CreateEntity();

	gMediator.AddComponent(
		mCamera,
		Transform{
			.position = glm::vec3(0.0f, 0.0f, 50.0f)
		});

	gMediator.AddComponent(
		mCamera,
		Camera{
			.projectionTransform = Camera::MakeProjectionTransform(45.0f, 0.1f, 1000.0f, 1920, 1080)
		});

	std::vector<glm::vec3> vertices =
		{
			glm::vec3(-0.5f, -0.5f, -0.5f),
			glm::vec3(0.5f, -0.5f, -0.5f),
			glm::vec3(0.5f, 0.5f, -0.5f),
			glm::vec3(0.5f, 0.5f, -0.5f),
			glm::vec3(-0.5f, 0.5f, -0.5f),
			glm::vec3(-0.5f, -0.5f, -0.5f),
			glm::vec3(-0.5f, -0.5f, 0.5),
			glm::vec3(0.5f, -0.5f, 0.5),
			glm::vec3(0.5f, 0.5f, 0.5),
			glm::vec3(0.5f, 0.5f, 0.5),
			glm::vec3(-0.5f, 0.5f, 0.5),
			glm::vec3(-0.5f, -0.5f, 0.5),
			glm::vec3(-0.5f, 0.5f, 0.5f),
			glm::vec3(-0.5f, 0.5f, -0.5f),
			glm::vec3(-0.5f, -0.5f, -0.5f),
			glm::vec3(-0.5f, -0.5f, -0.5f),
			glm::vec3(-0.5f, -0.5f, 0.5f),
			glm::vec3(-0.5f, 0.5f, 0.5f),
			glm::vec3(0.5f, 0.5f, 0.5),
			glm::vec3(0.5f, 0.5f, -0.5),
			glm::vec3(0.5f, -0.5f, -0.5),
			glm::vec3(0.5f, -0.5f, -0.5),
			glm::vec3(0.5f, -0.5f, 0.5),
			glm::vec3(0.5f, 0.5f, 0.5),
			glm::vec3(-0.5f, -0.5f, -0.5f),
			glm::vec3(0.5f, -0.5f, -0.5f),
			glm::vec3(0.5f, -0.5f, 0.5f),
			glm::vec3(0.5f, -0.5f, 0.5f),
			glm::vec3(-0.5f, -0.5f, 0.5f),
			glm::vec3(-0.5f, -0.5f, -0.5f),
			glm::vec3(-0.5f, 0.5f, -0.5),
			glm::vec3(0.5f, 0.5f, -0.5),
			glm::vec3(0.5f, 0.5f, 0.5),
			glm::vec3(0.5f, 0.5f, 0.5),
			glm::vec3(-0.5f, 0.5f, 0.5),
			glm::vec3(-0.5f, 0.5f, -0.5)
		};

	std::vector<glm::vec3> normals =
		{
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(-1.0f, 0.0f, 0.0f),
			glm::vec3(-1.0f, 0.0f, 0.0f),
			glm::vec3(-1.0f, 0.0f, 0.0f),
			glm::vec3(-1.0f, 0.0f, 0.0f),
			glm::vec3(-1.0f, 0.0f, 0.0f),
			glm::vec3(-1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0)
		};


	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);

	// Vertices
	glGenBuffers(1, &mVboVertices);
	glBindBuffer(GL_ARRAY_BUFFER, mVboVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)nullptr);
	glEnableVertexAttribArray(0);

	// Surface normal
	glGenBuffers(1, &mVboNormals);
	glBindBuffer(GL_ARRAY_BUFFER, mVboNormals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)nullptr);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}


void RenderSystem::Update(float dt)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // NOLINT (hicpp-signed-bitwise)

	shader->use();
	glBindVertexArray(mVao);


	auto& cameraTransform = gMediator.GetComponent<Transform>(mCamera);
	auto& camera = gMediator.GetComponent<Camera>(mCamera);

	for (auto const& entity : mEntities)
	{
		auto const& transform = gMediator.GetComponent<Transform>(entity);
		auto const& renderable = gMediator.GetComponent<Renderable>(entity);

		glm::mat4 view;
		view[0][3] = -cameraTransform.position.x;
		view[1][3] = -cameraTransform.position.y;
		view[2][3] = -cameraTransform.position.z;

		glm::mat4 rotY;

		float cos_theta_y = cosf(transform.rotation.y);
		float sin_theta_y = sinf(transform.rotation.y);

		rotY[0][0] = cos_theta_y;
		rotY[2][0] = -sin_theta_y;
		rotY[0][2] = sin_theta_y;
		rotY[2][2] = cos_theta_y;


		glm::mat4 rotX;

		float cosThetaX = cosf(transform.rotation.x);
		float sinThetaX = sinf(transform.rotation.x);

		rotX[1][1] = cosThetaX;
		rotX[2][1] = sinThetaX;
		rotX[1][2] = -sinThetaX;
		rotX[2][2] = cosThetaX;


		glm::mat4 rotZ;

		float cosThetaZ = cosf(transform.rotation.z);
		float sinThetaZ = sinf(transform.rotation.z);

		rotZ[0][0] = cosThetaZ;
		rotZ[1][0] = sinThetaZ;
		rotZ[0][1] = -sinThetaZ;
		rotZ[1][1] = cosThetaZ;


		glm::mat4 translate;
		translate[0][3] = transform.position.x;
		translate[1][3] = transform.position.y;
		translate[2][3] = transform.position.z;

		glm::mat4 scaleMat;
		scaleMat[0][0] = transform.scale.x;
		scaleMat[1][1] = transform.scale.y;
		scaleMat[2][2] = transform.scale.z;

		glm::mat4 model = translate * scaleMat * rotY;

		glm::mat4 projection = camera.projectionTransform;

		shader->setUniform<glm::mat4>("uModel", model);
		shader->setUniform<glm::mat4>("uView", view);
		shader->setUniform<glm::mat4>("uProjection", projection);
		shader->setUniform<glm::vec3>("uColor", renderable.color);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	glBindVertexArray(0);
}

void RenderSystem::WindowSizeListener(Event& event)
{
	auto windowWidth = event.GetParam<unsigned int>(Events::Window::Resized::WIDTH);
	auto windowHeight = event.GetParam<unsigned int>(Events::Window::Resized::HEIGHT);

	auto& camera = gMediator.GetComponent<Camera>(mCamera);
	camera.projectionTransform = Camera::MakeProjectionTransform(45.0f, 0.1f, 1000.0f, windowWidth, windowHeight);
}

