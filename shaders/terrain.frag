#version 330

#include "noise.glsl"


uniform vec3 lineColor;
uniform mat4 viewMatrix;
// uniform vec3 lightPosition;


out vec4 FragColor;
in vec2 noiseCoord;

// In view-space
in vec3 terrainNormal;
in vec3 terrainPos;

void main(void) {

    // NOTE: these only need to be here if we want to show noise as a texture
    // If we do, then these should probably be uniform or passed by the previous shader
    // as they need to be the same across shaders
    // float scale = 30.0f;
    // int octaves = 15;
    // float amplitude = 2.0;
    // float noise = fBm_noise(noiseCoord / scale, octaves, amplitude);

    // TODO: HARDCODED DIRECTIONAL LIGHT
    vec3 dirLightPosition = normalize(vec3(0.4, -0.6, 0.3));

    vec3 terrainColor = vec3(0.3); // Gray for now

    // lightning model
    vec3 n = normalize(terrainNormal);
    vec3 v = normalize(-terrainPos);
    vec3 l = normalize(-(mat3(viewMatrix) * dirLightPosition));
    vec3 h = normalize(l+v);
    float s = 400.0;

    vec3 ambient = 0.1f * terrainColor;
    vec3 diffuse = max(0.0, dot(n, l)) * terrainColor; 
    // vec3 specular = vec3(pow(max(0.0, dot(n, h)), s));

    FragColor = vec4(vec3(ambient + diffuse), 1.0);

    if (lineColor != vec3(0.0)) {
        FragColor = vec4(vec3(lineColor), 1.0);
    }
}
