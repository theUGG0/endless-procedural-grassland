#version 330

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
// Here we could give a name to attribute 0
// from our vertex attribute array.
// layout(location = 0) in vec3 position;
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 instanceOffset;

// in vec3 position;
in vec3 color;
out vec3 interpolatedColor;

void main(void) {
    interpolatedColor = color;
    vec3 worldPosition = position + instanceOffset;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(worldPosition, 1.0);
}
