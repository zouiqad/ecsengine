#pragma once
#define _USE_MATH_DEFINES

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>>
#include <math.h>

struct Camera
{
    glm::mat4 projectionTransform;

    static glm::mat4 MakeProjectionTransform(float fov, float nearClip, float farClip, unsigned int windowWidth, unsigned int windowHeight)
    {
        float zClipBias0 =
            (farClip + nearClip)
            / (farClip - nearClip);

        float zClipBias1 =
            (2.0f * farClip * nearClip)
            / (farClip - nearClip);

        float zoomX = 1.0f / tanf((fov / 2.0f) * (M_PI / 180.0f));
        float zoomY = (zoomX * static_cast<float>(windowWidth)) / static_cast<float>(windowHeight);

        glm::mat4 transform = glm::mat4(1.0f);
        transform[0][0] = zoomX;
        transform[1][1] = zoomY;
        transform[2][2] = -zClipBias0;
        transform[3][2] = -1;
        transform[2][3] = -zClipBias1;

        return glm::perspective (fov, 16.0f / 9.0f, nearClip, farClip);
        //return transform;
    }
};
