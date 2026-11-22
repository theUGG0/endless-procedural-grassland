#version 330

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

uniform float baseHeight;
uniform float baseWidth;
uniform float baseTilt;
uniform float baseBend;
// Here we could give a name to attribute 0
// from our vertex attribute array.
// layout(location = 0) in vec3 position;
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 instanceOffset;

// in vec3 position;
out vec3 interpolatedColor;

void main(void) {
    // some inspo from here https://gpuopen.com/learn/mesh_shaders/mesh_shaders-procedural_grass_rendering/
    // the base values should have randomness applied to them based on the coordinate hash and their Voronoi clump parameters 
    vec3 P0 = vec3(0.0,0.0,0.0);
    vec3 P3 = vec3(0.0, baseHeight * cos(baseTilt), baseHeight * sin(baseTilt));
    vec3 normal = normalize(vec3(0.0, P3.z, -P3.y));
    vec3 P1 = mix(P0, P3, 1.0/3.0) + normal * baseBend * baseHeight / 2;
    vec3 P2 = mix(P0, P3, 2.0/3.0) + normal * baseBend * baseHeight;

    float t = position.x;
    float u = 1 - position.x;

    interpolatedColor = vec3(1.0, 1.0, 1.0);
    vec3 calculatedPos = (u*u*u*P0+
                          3*u*u*t*P1+
                          3*u*t*t*P2+
                          t*t*t*P3);
    calculatedPos.x += position.y * baseWidth * u;
    vec3 worldPosition = calculatedPos + instanceOffset;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(worldPosition, 1.0);
}
