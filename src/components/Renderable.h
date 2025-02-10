#pragma once

#include"graphics/Model.h"
#include <glm/glm.hpp>

struct Renderable
{
    std::shared_ptr<Model> model;
    glm::vec3 color;
};
