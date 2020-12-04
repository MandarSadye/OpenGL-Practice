#pragma once
#include <string>

#include "glm/vec4.hpp"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.hpp"

#include "Application.hpp"

class NODE {

    private:

    VertexArray va;
    VertexBufferLayout layout;
    VertexBuffer vb;
    IndexBuffer ib;
    Renderer renderer;

    public: 

    Shader shader;
    uint32_t id_next;
    std::string ID;

    std::vector<std::string> childIDs;
    std::string NAME;
    std::string PARENT_ID;
    uint32_t VERTEX_COUNT;
    uint32_t VERTEX_SIZE;
    float *VERTEX;
    std::string ELEMENT_TYPE;
    uint32_t ELEMENT_COUNT;
    uint32_t *ELEMENT;
    bool rotateX;
    bool rotateY;
    bool rotateZ;
    bool translateX;
    bool translateY;
    bool translateZ;

    glm::vec4 ROTATION;
    glm::vec4 TRANSLATION;
    glm::vec4 SCALE;
    glm::vec4 PARENT_MOUNT;

    glm::mat4 rotation;
    glm::mat4 translation;
    glm::mat4 scale;

    NODE(std::string id, std::string name, std::string parent_id, uint32_t cnt_vertex, uint32_t cnt_element,
            float * vectex, uint32_t * element, uint32_t vertex_size, std::string elem_type,
            bool rx, bool ry, bool rz,
            bool tx, bool ty, bool tz,
            glm::vec4 rotate, glm::vec4 translate, glm::vec4 scale, glm::vec4 parent_mount,
            std::vector<std::string> &filenames, std::vector<std::string> &typeOfShader);

    void updateMatrices();
    glm::mat4 multiply_stack(const std::vector<glm::mat4> &matStack);
    void render();

    void AddChild();
    bool check = false;
    static void insertGeomery(std::string parent_id, std::string filename, int32_t parent);

    static uint32_t getPrimitiveSize(std::string primitive) {
        if (primitive == "GL_TRIANGLES") return 3;
        else if (primitive == "GL_LINE_STRIP") return 1;
        return 0;
    }

    static uint32_t getPrimitiveID(std::string primitive) {
        if (primitive == "GL_TRIANGLES") return GL_TRIANGLES;
        else if (primitive == "GL_LINE_STRIP") return GL_LINE_STRIP;
        return 0;
    }

};