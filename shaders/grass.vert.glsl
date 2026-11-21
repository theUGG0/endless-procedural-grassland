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
out vec3 interpolatedColor;

void main(void) {
    vec3 P0 = vec3(0.0,0.0,0.0);
    vec3 P1 = vec3(0.0,1.5,0.0);
    vec3 P2 = vec3(0.0,1.9,0.2);
    vec3 P3 = vec3(0.0,1.8,0.5);

    float t = position.x;
    float u = 1 - position.x;

    interpolatedColor = vec3(1.0, 1.0, 1.0);
    vec3 calculatedPos = (u*u*u*P0+
                          3*u*u*t*P1+
                          3*u*t*t*P2+
                          t*t*t*P3);
    calculatedPos += position.y * 0.05 * u;
    vec3 worldPosition = calculatedPos + instanceOffset;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(worldPosition, 1.0);
}
