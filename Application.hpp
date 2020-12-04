#pragma once
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
// Defining the ESCAPE Key Code
#define ESCAPE 27
// Defining the DELETE Key Code
#define DELETE 127

// C++ HEADERS
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

// CUSTOM LIB HEADERS
#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.hpp"

// GLM HEADERS
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

// MODEL HEADERS
#include "camera.hpp"
#include "node.hpp"

class NODE;
class Application {
    private:
    static Application* instance;

    GLFWwindow* window;
    uint window_width;
    uint window_height;
    float aspect_ratio;
    
    glm::mat4 rotation_matrix;
    glm::mat4 projection_matrix;
    glm::mat4 c_rotation_matrix;
    glm::mat4 lookat_matrix;
    glm::mat4 model_matrix;
    glm::mat4 view_matrix;
    glm::mat4 modelview_matrix;

    int error = 0;

    void init();
    GLFWwindow* InitWindow();
    void parseConfig();
    void renderGL();

    static void error_callback(int error, const char* description);
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void mouse_callback(GLFWwindow* window, double x, double y);
    static void mouse_enter_callback(GLFWwindow* window, int enter);
    static bool entered_mouse;

    void configCameraLookAt();

    public:


    bool enable_culling = true;
    bool solid = true;
    bool enable_perspective = false;
    std::map<std::string, NODE*> geometry = std::map<std::string, NODE*>();

    std::vector<camera*> vecCamera;
    int active_camera = 0;

    std::vector<glm::mat4> matrixStack;

    static Application* getInstance();

    void run();
    void ClearAll();
    void rotateCameraOnMouse(int x, int y);
};