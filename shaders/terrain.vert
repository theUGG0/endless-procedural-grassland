#version 460 core

uniform mat4 modelMatrix;

in vec3 position;
out vec2 vertexUV;

void main() {
    vec4 world = modelMatrix * vec4(position, 1.0); // Calculate terrain "world" UV from position
    vertexUV = world.xz;    
    gl_Position = vec4(position, 1.0);
}
