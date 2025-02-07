#include "RenderSystem.h"

#include "components/Camera.h"
#include "components/Renderable.h"
#include "components/Transform.h"
#include "core/Mediator.h"
#include "graphics/Shader.h"
#include <cmath>


extern Mediator gMediator;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

void RenderSystem::Init()
{
	gMediator.AddEventListener(METHOD_LISTENER(Events::Window::RESIZED, RenderSystem::WindowSizeListener));

	shader = std::make_unique<Shader>("resources/shaders/simple.vert", "resources/shaders/simple.frag");

	mCamera = gMediator.CreateEntity();

	gMediator.AddComponent(
		mCamera,
		Transform{
			.position = glm::vec3(0.0f, 0.0f, 10.0f)
		});

	gMediator.AddComponent(
		mCamera,
		Camera{
			.projectionTransform = Camera::MakeProjectionTransform(45.0f, 0.1f, 1000.0f, 1920, 1080)
		});

	// Cube vertices: 36 positions (3 coordinates per vertex)
	std::vector positions = {
		// Front face (normal:  0,  0,  1)
		-0.5f, -0.5f,  0.5f,  // bottom-left
		 0.5f, -0.5f,  0.5f,  // bottom-right
		 0.5f,  0.5f,  0.5f,  // top-right

		-0.5f, -0.5f,  0.5f,  // bottom-left
		 0.5f,  0.5f,  0.5f,  // top-right
		-0.5f,  0.5f,  0.5f,  // top-left

		// Right face (normal: 1,  0,  0)
		 0.5f, -0.5f,  0.5f,  // bottom-front
		 0.5f, -0.5f, -0.5f,  // bottom-back
		 0.5f,  0.5f, -0.5f,  // top-back

		 0.5f, -0.5f,  0.5f,  // bottom-front
		 0.5f,  0.5f, -0.5f,  // top-back
		 0.5f,  0.5f,  0.5f,  // top-front

		// Back face (normal: 0,  0, -1)
		 0.5f, -0.5f, -0.5f,  // bottom-right
		-0.5f, -0.5f, -0.5f,  // bottom-left
		-0.5f,  0.5f, -0.5f,  // top-left

		 0.5f, -0.5f, -0.5f,  // bottom-right
		-0.5f,  0.5f, -0.5f,  // top-left
		 0.5f,  0.5f, -0.5f,  // top-right

		// Left face (normal: -1,  0,  0)
		-0.5f, -0.5f, -0.5f,  // bottom-back
		-0.5f, -0.5f,  0.5f,  // bottom-front
		-0.5f,  0.5f,  0.5f,  // top-front

		-0.5f, -0.5f, -0.5f,  // bottom-back
		-0.5f,  0.5f,  0.5f,  // top-front
		-0.5f,  0.5f, -0.5f,  // top-back

		// Top face (normal: 0,  1,  0)
		-0.5f,  0.5f,  0.5f,  // front-left
		 0.5f,  0.5f,  0.5f,  // front-right
		 0.5f,  0.5f, -0.5f,  // back-right

		-0.5f,  0.5f,  0.5f,  // front-left
		 0.5f,  0.5f, -0.5f,  // back-right
		-0.5f,  0.5f, -0.5f,  // back-left

		// Bottom face (normal: 0, -1,  0)
		-0.5f, -0.5f, -0.5f,  // back-left
		 0.5f, -0.5f, -0.5f,  // back-right
		 0.5f, -0.5f,  0.5f,  // front-right

		-0.5f, -0.5f, -0.5f,  // back-left
		 0.5f, -0.5f,  0.5f,  // front-right
		-0.5f, -0.5f,  0.5f   // front-left
	};


	// Cube normals: 36 normals (3 components per normal)
	std::vector normals = {
		// Front face normals (0, 0, 1)
		0.0f, 0.0f, 1.0f,  // for vertex 1
		0.0f, 0.0f, 1.0f,  // for vertex 2
		0.0f, 0.0f, 1.0f,  // for vertex 3

		0.0f, 0.0f, 1.0f,  // for vertex 4
		0.0f, 0.0f, 1.0f,  // for vertex 5
		0.0f, 0.0f, 1.0f,  // for vertex 6

		// Right face normals (1, 0, 0)
		1.0f, 0.0f, 0.0f,  // for vertex 7
		1.0f, 0.0f, 0.0f,  // for vertex 8
		1.0f, 0.0f, 0.0f,  // for vertex 9

		1.0f, 0.0f, 0.0f,  // for vertex 10
		1.0f, 0.0f, 0.0f,  // for vertex 11
		1.0f, 0.0f, 0.0f,  // for vertex 12

		// Back face normals (0, 0, -1)
		0.0f, 0.0f, -1.0f, // for vertex 13
		0.0f, 0.0f, -1.0f, // for vertex 14
		0.0f, 0.0f, -1.0f, // for vertex 15

		0.0f, 0.0f, -1.0f, // for vertex 16
		0.0f, 0.0f, -1.0f, // for vertex 17
		0.0f, 0.0f, -1.0f, // for vertex 18

		// Left face normals (-1, 0, 0)
	   -1.0f, 0.0f, 0.0f,  // for vertex 19
	   -1.0f, 0.0f, 0.0f,  // for vertex 20
	   -1.0f, 0.0f, 0.0f,  // for vertex 21

	   -1.0f, 0.0f, 0.0f,  // for vertex 22
	   -1.0f, 0.0f, 0.0f,  // for vertex 23
	   -1.0f, 0.0f, 0.0f,  // for vertex 24

		// Top face normals (0, 1, 0)
		0.0f, 1.0f, 0.0f,  // for vertex 25
		0.0f, 1.0f, 0.0f,  // for vertex 26
		0.0f, 1.0f, 0.0f,  // for vertex 27

		0.0f, 1.0f, 0.0f,  // for vertex 28
		0.0f, 1.0f, 0.0f,  // for vertex 29
		0.0f, 1.0f, 0.0f,  // for vertex 30

		// Bottom face normals (0, -1, 0)
		0.0f, -1.0f, 0.0f, // for vertex 31
		0.0f, -1.0f, 0.0f, // for vertex 32
		0.0f, -1.0f, 0.0f, // for vertex 33

		0.0f, -1.0f, 0.0f, // for vertex 34
		0.0f, -1.0f, 0.0f, // for vertex 35
		0.0f, -1.0f, 0.0f  // for vertex 36
	};

	std::vector<Vertex> vertices;
	vertices.reserve(36);
	for (size_t i = 0; i < 36; i++) {
		Vertex v;
		v.Position = glm::vec3(positions[i * 3 + 0], positions[i * 3 + 1], positions[i * 3 + 2]);
		v.Normal   = glm::vec3(normals[i * 3 + 0], normals[i * 3 + 1], normals[i * 3 + 2]);
		vertices.emplace_back(v);
	}


	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData (GL_ARRAY_BUFFER, vertices.size () * sizeof (Vertex), &vertices[0], GL_STATIC_DRAW);

	// Vertices
	glEnableVertexAttribArray (0);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, sizeof (Vertex), (void*)0);

	// Surface normal
	glEnableVertexAttribArray (1);
	glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, sizeof (Vertex),
		(void*)offsetof (Vertex, Normal));

	glBindVertexArray(0);
}


void RenderSystem::Update(float dt)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // NOLINT (hicpp-signed-bitwise)

	shader->use();
	glBindVertexArray(mVAO);


	auto& cameraTransform = gMediator.GetComponent<Transform>(mCamera);
	auto& camera = gMediator.GetComponent<Camera>(mCamera);

	for (auto const& entity : mEntities)
	{
		auto const& transform = gMediator.GetComponent<Transform>(entity);
		auto const& renderable = gMediator.GetComponent<Renderable>(entity);

		glm::mat4 view = glm::mat4(1.0f);
		view[0][3] = -cameraTransform.position.x;
		view[1][3] = -cameraTransform.position.y;
		view[2][3] = -cameraTransform.position.z;

		glm::mat4 rotY = glm::mat4(1.0f);

		float cos_theta_y = cosf(transform.rotation.y);
		float sin_theta_y = sinf(transform.rotation.y);

		rotY[0][0] = cos_theta_y;
		rotY[2][0] = -sin_theta_y;
		rotY[0][2] = sin_theta_y;
		rotY[2][2] = cos_theta_y;


		glm::mat4 rotX = glm::mat4(1.0f);

		float cosThetaX = cosf(transform.rotation.x);
		float sinThetaX = sinf(transform.rotation.x);

		rotX[1][1] = cosThetaX;
		rotX[2][1] = sinThetaX;
		rotX[1][2] = -sinThetaX;
		rotX[2][2] = cosThetaX;


		glm::mat4 rotZ = glm::mat4(1.0f);

		float cosThetaZ = cosf(transform.rotation.z);
		float sinThetaZ = sinf(transform.rotation.z);

		rotZ[0][0] = cosThetaZ;
		rotZ[1][0] = sinThetaZ;
		rotZ[0][1] = -sinThetaZ;
		rotZ[1][1] = cosThetaZ;


		glm::mat4 translate = glm::mat4(1.0f);
		translate[0][3] = transform.position.x;
		translate[1][3] = transform.position.y;
		translate[2][3] = transform.position.z;

		glm::mat4 scaleMat = glm::mat4(1.0f);
		scaleMat[0][0] = transform.scale.x;
		scaleMat[1][1] = transform.scale.y;
		scaleMat[2][2] = transform.scale.z;

		glm::mat4 model = translate * scaleMat * rotY;

		glm::mat4 projection = camera.projectionTransform;

		view = glm::lookAt(cameraTransform.position, cameraTransform.position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		shader->setUniform<glm::mat4>("uModel", glm::mat4(1.0f));
		shader->setUniform<glm::mat4>("uView", view);
		shader->setUniform<glm::mat4>("uProj", projection);
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

