#pragma once

// GLM HEADERS
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

class camera {
    public: 
    int ID;
    double WINDOW_HEIGHT_FACTOR;
    double FRUSTUM_NEAR;
    double FRUSTUM_FAR;
    double ORTH_NEAR;
    double ORTH_FAR;
    glm::vec4 POSITION;
    glm::vec4 UPVECTOR;
    glm::vec4 ROTATION;
};