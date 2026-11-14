#version 330

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
// Here we could give a name to attribute 0
// from our vertex attribute array.
// layout(location = 0) in vec3 position;

in vec3 position;
in vec3 color;
in vec2 texCoords;

out vec3 interpolatedColor;
out vec2 uv;

void main(void) {
    interpolatedColor = color;
    uv = texCoords;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
}
