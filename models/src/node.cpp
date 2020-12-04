#include "node.hpp"

NODE::NODE(std::string id, std::string name, std::string parent_id, uint32_t cnt_vertex, uint32_t cnt_element,
            float * fragment, uint32_t * element, uint32_t vertex_size, std::string elem_type,
            bool rx, bool ry, bool rz,
            bool tx, bool ty, bool tz,
            glm::vec4 rotate, glm::vec4 translate, glm::vec4 scale, glm::vec4 parent_mount,
            std::vector<std::string> &filenames, std::vector<std::string> &typeOfShader) : va() , 
            vb(fragment, cnt_vertex * vertex_size * sizeof(float)), ib(element, cnt_element * NODE::getPrimitiveSize(elem_type)),
            shader(filenames, typeOfShader), childIDs(std::vector<std::string>())
            {

    
    

    ID = id;
    id_next = 0;
    NAME = name;
    PARENT_ID = parent_id;
    VERTEX_COUNT = cnt_vertex; ELEMENT_COUNT = cnt_element;
    VERTEX_SIZE = vertex_size;
    VERTEX = fragment; ELEMENT = element; 
    ELEMENT_TYPE = elem_type;
    rotateX = rx; rotateY = ry; rotateZ = rz;
    translateX = tx; translateY = ty; translateZ = tz;
    ROTATION = rotate; TRANSLATION = translate; SCALE = scale; PARENT_MOUNT = parent_mount;
    shader.Bind();

    layout = VertexBufferLayout();
    layout.AddFloat(4);
    layout.AddFloat(4);

    va.AddBuffer(vb, layout);

    renderer = Renderer();
    if (parent_id != "")
    Application::getInstance()->geometry[parent_id]->childIDs.push_back(id);
    Application::getInstance()->ClearAll();

    updateMatrices();
}

glm::mat4 NODE::multiply_stack(const std::vector<glm::mat4> &matStack){
    glm::mat4 mult = glm::mat4(1.0f);

    for(int i=0;i<matStack.size();i++){
        mult = mult * matStack[i];
    }	

    return mult;
}

void NODE::render() {

    if (!check) {
    }
    updateMatrices();
    glm::mat4 mvp = Application::getInstance()->matrixStack[0];
    std::string c_id = ID;
    while (c_id != "") {
        NODE* c_node = Application::getInstance()->geometry[c_id];
        mvp *= (c_node->translation * c_node->rotation);
        c_id = c_node->PARENT_ID;
    }
    mvp *= scale;
    shader.SetUniformMat4f("u_MVP", mvp); 

    renderer.draw(va, ib, shader, getPrimitiveID(ELEMENT_TYPE));
}

void NODE::updateMatrices() {
    rotation = glm::rotate(glm::mat4(1.0f), glm::radians(ROTATION.x), glm::vec3(1.0f,0.0f,0.0f));
    rotation = glm::rotate(rotation, glm::radians(ROTATION.y), glm::vec3(0.0f,1.0f,0.0f));
    rotation = glm::rotate(rotation, glm::radians(ROTATION.z), glm::vec3(0.0f,0.0f,1.0f));

    translation = glm::translate(glm::mat4(1.0f),glm::vec3(TRANSLATION.x, TRANSLATION.y, TRANSLATION.z));

    scale = glm::scale(glm::mat4(1.0f),glm::vec3(SCALE.x,SCALE.y,SCALE.z));
}

void NODE::AddChild() {
    
}

void NODE::insertGeomery(std::string parent_id, std::string filename, int32_t parent) {
    std::string line;
    std::ifstream file(filename, std::ios::in); 
    std::istringstream ss;
    
    if (file.good()) {
        while (true) {
            uint32_t id_num;
            uint32_t id_next;
            uint32_t pid;
            std::string ID;

            
            std::string NAME;
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
            std::string key;

            std::vector<std::string> mfilename = std::vector<std::string>();
            std::vector<std::string> mshader_type = std::vector<std::string>();

            if (!getline(file,line)) break; if (line != "// NEW_SEG_START") continue;
            if (!getline(file,line)) break; ss = std::istringstream(line); ss >> key; if(key == "ID:") ss >> id_num;
            if (!getline(file,line)) break; ss = std::istringstream(line); ss >> key; if(key == "NAME:") ss >> NAME;
            if (!getline(file,line)) break; ss = std::istringstream(line); ss >> key; if(key == "PARENT:") ss >> pid;
            if (pid != parent) continue;
            if (!getline(file,line)) break; ss = std::istringstream(line); ss >> key; if(key == "VERTEX_COUNT:") ss >> VERTEX_COUNT;
            if (!getline(file,line)) break; ss = std::istringstream(line); ss >> key; if(key == "VERTEX_SIZE:") ss >> VERTEX_SIZE;
            if (!getline(file,line)) break; 
            VERTEX = new float[VERTEX_COUNT*VERTEX_SIZE];
            for (int i = 0; i < VERTEX_COUNT; i++) {
                if (!getline(file,line)) break; 
                ss = std::istringstream(line);
                for (int j = 0; j < VERTEX_SIZE; j++) ss >> VERTEX[i*VERTEX_SIZE + j];
            }
            if (!getline(file,line)) break; ss = std::istringstream(line); ss >> key; if(key == "ELEMENT_TYPE:") ss >> ELEMENT_TYPE;
            if (!getline(file,line)) break; ss = std::istringstream(line); ss >> key; if(key == "ELEMENT_COUNT:") ss >> ELEMENT_COUNT;
            if (!getline(file,line)) break; 
            ELEMENT = new uint32_t[ELEMENT_COUNT * getPrimitiveSize(ELEMENT_TYPE) ];
            for (int i = 0; i < ELEMENT_COUNT; i++) {
                if (!getline(file,line)) break; ss = std::istringstream(line); 
                for (int j = 0; j < getPrimitiveSize(ELEMENT_TYPE); j++) ss >> ELEMENT[i*getPrimitiveSize(ELEMENT_TYPE) + j];
            }
            if (!getline(file,line)) break; 
            if (!getline(file,line)) break; 
            if (!getline(file,line)) break; ss = std::istringstream(line); ss >> key; if(key == "X:") ss >> rotateX;
            if (!getline(file,line)) break; ss = std::istringstream(line); ss >> key; if(key == "Y:") ss >> rotateY;
            if (!getline(file,line)) break; ss = std::istringstream(line); ss >> key; if(key == "Z:") ss >> rotateZ;
            if (!getline(file,line)) break; 
            if (!getline(file,line)) break; ss = std::istringstream(line); ss >> key; if(key == "X:") ss >> translateX;
            if (!getline(file,line)) break; ss = std::istringstream(line); ss >> key; if(key == "Y:") ss >> translateY;
            if (!getline(file,line)) break; ss = std::istringstream(line); ss >> key; if(key == "Z:") ss >> translateZ;
            
            if (!getline(file,line)) { break; }
            if (!getline(file,line)) break; ss = std::istringstream(line); ss >> key; if(key == "ROTATION:")ss >> ROTATION.x >> ROTATION.y >> ROTATION.z >> ROTATION.w; 
            
            if (!getline(file,line)) break; ss = std::istringstream(line); ss >> key; if(key == "TRANSLATION:")ss >> TRANSLATION.x >> TRANSLATION.y >> TRANSLATION.z >> TRANSLATION.w; 
            if (!getline(file,line)) break; ss = std::istringstream(line); ss >> key; if(key == "SCALE:")ss >> SCALE.x >> SCALE.y >> SCALE.z >> SCALE.w; 
            if (!getline(file,line)) break; ss = std::istringstream(line); ss >> key; if(key == "PARENT_MOUNT:")ss >> PARENT_MOUNT.x >> PARENT_MOUNT.y >> PARENT_MOUNT.z >> PARENT_MOUNT.w; 
            if (!getline(file,line)) break; ss = std::istringstream(line); ss >> key; if(key == "SHADER:") {
                while (true) {
                if (!getline(file,line)) break; ss = std::istringstream(line); ss >> key; if(key == "SHADER_END") break;
                mshader_type.push_back(key); ss >> key; mfilename.push_back(key);
                }
            }
            NODE* temp = new NODE((parent_id == "") ? std::to_string(0) : parent_id + ":" + std::to_string(Application::getInstance()->geometry[parent_id]->id_next++),
            NAME, parent_id, VERTEX_COUNT, ELEMENT_COUNT, VERTEX, ELEMENT, VERTEX_SIZE, ELEMENT_TYPE,
            rotateX, rotateY, rotateZ, translateX, translateY, translateZ,
            ROTATION, TRANSLATION, SCALE, PARENT_MOUNT,
            mfilename, mshader_type);
            Application::getInstance()->geometry[temp->ID] = temp;
            insertGeomery(temp->ID, filename, id_num);
            if (!getline(file,line)) break; 
        }
    }
    else {
        std::cout << "File error in NODE::InsertGeometry\n" << std::endl; 
    }

}