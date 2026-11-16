#version 330

#include "noise.glsl"


uniform vec3 lineColor;
uniform mat4 viewMatrix;
// uniform vec3 lightPosition;


out vec4 FragColor;
in vec2 tessEvalUV;
in vec3 terrainNormal;
in vec3 viewPos;

void main(void) {

    // TODO: these should probably be uniform as they need to be the same across shaders anyways and some rely on CPU side info
    float sizeOfChunk = 20.0f;
    int octaves = 4;
    float amplitude = 2.0;
    float noise = fBm_noise(tessEvalUV / sizeOfChunk, octaves, amplitude);

    // TODO: HARDCODED DIRECTIONAL LIGHT
    vec3 dirLightPosition = normalize(vec3(0.4, -0.6, 0.3));

    vec3 terrainColor = vec3(0.3); // Gray for now

    // lightning model
    vec3 n = normalize(terrainNormal);
    vec3 v = normalize(-viewPos);
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
