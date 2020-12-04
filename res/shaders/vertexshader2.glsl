#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 in_color;

uniform mat4 u_MVP;

out VS2 {
    vec4 color;
} to_fs;

void main() {
    gl_Position = u_MVP * position;
    to_fs.color = in_color;
};