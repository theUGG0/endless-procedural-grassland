#version 460 core

uniform mat4 modelMatrix;

in vec3 position;
void main() {
    gl_Position = vec4(position, 1.0);
}
