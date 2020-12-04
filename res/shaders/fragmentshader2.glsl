#version 330 core
// #extension GL_ARB_separate_shader_objects : enable
out vec4 out_color;

in VS2
{
   vec4 color;
} from_vs;

void main() {
    out_color = from_vs.color;
};