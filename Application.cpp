#include "Application.hpp"
Application* Application::instance = nullptr;
bool Application::entered_mouse = false;

Application* Application::getInstance() {
    if (instance == nullptr) 
    {
        instance = new Application;
        instance->init();
    }
    return instance;
}

void Application::init() {
    matrixStack = std::vector<glm::mat4>();
    vecCamera = std::vector<camera*>();
    vecCamera.push_back(new camera);
    vecCamera.push_back(new camera);
    vecCamera.push_back(new camera);


    parseConfig();
    if (InitWindow() == nullptr) {
        error = 1;
        return;
    };

    //Set framebuffer clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //Set depth buffer furthest depth
    glClearDepth(1.0);
    //Set depth test to less-than
    glDepthFunc(GL_LESS);
    //Enable depth testing
    glEnable(GL_DEPTH_TEST);


    configCameraLookAt();
    NODE::insertGeomery("", "res/data/cube.txt", -1);

}

void Application::configCameraLookAt() {
    matrixStack.clear();

    c_rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(vecCamera[active_camera]->ROTATION.x), glm::vec3(1.0f,0.0f,0.0f));
    c_rotation_matrix = glm::rotate(c_rotation_matrix, glm::radians(vecCamera[active_camera]->ROTATION.y), glm::vec3(0.0f,1.0f,0.0f));
    c_rotation_matrix = glm::rotate(c_rotation_matrix, glm::radians(vecCamera[active_camera]->ROTATION.z), glm::vec3(0.0f,0.0f,1.0f));

    glm::vec4 c_pos = glm::vec4(vecCamera[active_camera]->POSITION.x,vecCamera[active_camera]->POSITION.y,vecCamera[active_camera]->POSITION.z, 1.0)*c_rotation_matrix;
    glm::vec4 c_up = glm::vec4(vecCamera[active_camera]->UPVECTOR.x,vecCamera[active_camera]->UPVECTOR.y,vecCamera[active_camera]->UPVECTOR.z, 1.0)*c_rotation_matrix;
    lookat_matrix = glm::lookAt(glm::vec3(c_pos),glm::vec3(0.0),glm::vec3(c_up));

    double sem_height = vecCamera[active_camera]->WINDOW_HEIGHT_FACTOR;
    double sem_width = sem_height * aspect_ratio;
    if(enable_perspective)
        projection_matrix = glm::frustum(-sem_width, sem_width, -sem_height, sem_height, 
        vecCamera[active_camera]->FRUSTUM_NEAR, vecCamera[active_camera]->FRUSTUM_FAR);
    else
    projection_matrix = glm::ortho(-sem_width, sem_width, -sem_height, sem_height, 
        vecCamera[active_camera]->ORTH_NEAR, vecCamera[active_camera]->ORTH_FAR);

    view_matrix = projection_matrix*lookat_matrix;

}

GLFWwindow* Application::InitWindow()
{
    glfwSetErrorCallback(Application::error_callback);

    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return nullptr;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( window_width, window_height, "RACE_TRACK", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return nullptr;

    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return nullptr;
    }

    std::cout << "Using GL Version: " << glGetString(GL_VERSION) << std::endl;

    //Print and see what context got enabled
    std::cout<<"Vendor: "<<glGetString (GL_VENDOR)<<std::endl;
    std::cout<<"Renderer: "<<glGetString (GL_RENDERER)<<std::endl;
    std::cout<<"Version: "<<glGetString (GL_VERSION)<<std::endl;
    std::cout<<"GLSL Version: "<<glGetString (GL_SHADING_LANGUAGE_VERSION)<<std::endl;

    //Keyboard Callback
    glfwSetKeyCallback(window, Application::key_callback);
    //Framebuffer resize callback
    glfwSetFramebufferSizeCallback(window, Application::framebuffer_size_callback);
    // Mouse callback
    glfwSetCursorEnterCallback(window, mouse_enter_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    return window;
}

void Application::ClearAll()
{
    GLCall( glUseProgram(0) );
    GLCall( glBindBuffer(GL_ARRAY_BUFFER, 0) );
    GLCall( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) );
    GLCall( glBindVertexArray(0) );
}

void Application::parseConfig() {
    std::ifstream file("config/config.txt", std::ios::in); 
    std::string line;
    int index = -1;
    if (file.good()) {
        while (getline(file, line)) {
            std::istringstream ss(line);
            std::string key;
            switch (index) {
                case -1:
                ss >> key;
                if (key == "WINDOW_WIDTH:") ss >> window_width;
                else if (key == "WINDOW_HEIGHT:") ss >> window_height;
                break;
                case 0:
                case 1:
                case 2:
                ss >> key;
                if (key == "ID:") ss >> vecCamera[index]->ID;
                else if (key == "WINDOW_WIDTH_FACTOR:") ss >>  vecCamera[index]->WINDOW_HEIGHT_FACTOR;
                else if (key == "FRUSTUM_NEAR:")        ss >>  vecCamera[index]->FRUSTUM_NEAR;
                else if (key == "FRUSTUM_FAR:")         ss >>  vecCamera[index]->FRUSTUM_FAR;
                else if (key == "ORTH_NEAR:")           ss >>  vecCamera[index]->ORTH_NEAR;
                else if (key == "ORTH_FAR:")            ss >>  vecCamera[index]->ORTH_FAR;
                else if (key == "POSITION:")            ss >>  vecCamera[index]->POSITION.x >>  vecCamera[index]->POSITION.y >>  vecCamera[index]->POSITION.z >>  vecCamera[index]->POSITION.w;
                else if (key == "UPVECTOR:")            ss >>  vecCamera[index]->UPVECTOR.x >>  vecCamera[index]->UPVECTOR.y >>  vecCamera[index]->UPVECTOR.z >>  vecCamera[index]->UPVECTOR.w;
                else if (key == "ROTATION:")              ss >>  vecCamera[index]->ROTATION.x >>  vecCamera[index]->ROTATION.y >>  vecCamera[index]->ROTATION.z >>  vecCamera[index]->ROTATION.w;
            }
            if (line == "// CAMERA_START") index++;
        }
    }
    else {
        std::cout << "File error in Applications::parseConfig\n";
    }

    aspect_ratio = (float)window_width / (float)window_height;
}

void Application::error_callback(int error, const char* description) {
    std::cerr<<description<<std::endl;
}

void Application::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, GL_TRUE); break;
            case GLFW_KEY_P: 
            Application::getInstance()->enable_perspective = !Application::getInstance()->enable_perspective;
            Application::getInstance()->configCameraLookAt();
            break;
        }
    } 
}

void Application::mouse_callback(GLFWwindow* window, double x, double y) {
    if (!entered_mouse) return;
    int diffx = Application::getInstance()->window_width/2  - x;
    int diffy = Application::getInstance()->window_height/2 - y;
    Application::getInstance()->rotateCameraOnMouse(diffx, diffy);
    glfwSetCursorPos(window, Application::getInstance()->window_width/2, Application::getInstance()->window_height/2);
}

void Application::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (button == GLFW_MOUSE_BUTTON_1) std::cout << "Left Button Pressed" << std::endl;
        if (button == GLFW_MOUSE_BUTTON_2) std::cout << "Right Button Pressed" << std::endl;
    }
}

void Application::mouse_enter_callback(GLFWwindow* window, int enter) {
    if (enter) entered_mouse = true;
    else entered_mouse = false;
    std::cout << "Mouse " << ((enter == true) ? "Entered" : "Exitted") << " Window" << std::endl;
}
void Application::rotateCameraOnMouse(int x, int y) {
    vecCamera[active_camera]->ROTATION.y += x;
    vecCamera[active_camera]->ROTATION.x += y;
    configCameraLookAt();
}

void Application::run() {
    while (glfwWindowShouldClose(window) == 0)
    {
        // Render here
        renderGL();
        // Swap front and back buffers
        glfwSwapBuffers(window);
        // Poll for and process events
        glfwPollEvents();
    }
    glfwTerminate();
}

void Application::renderGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    matrixStack.clear();
    matrixStack.push_back(view_matrix);
    for (auto &i: geometry)
    i.second->render();
}

int main() {
    Application::getInstance()->run();
}