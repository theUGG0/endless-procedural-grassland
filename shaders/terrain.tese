#version 460 core

#include "noise.glsl"

// Based on this video
// https://www.youtube.com/watch?v=21gfE-zUym8&t

layout (quads, fractional_odd_spacing, ccw) in;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

in vec2 tessControlUV[];
out vec2 tessEvalUV;
out vec3 viewPos;
out vec3 terrainNormal;

const float HEIGHT_AMPLITUDE = 20.0;

float sampleNoise(vec2 uv) {
    float scale = 30.0f; 
    float amplitude = 2000.0f;
    int octaves = 15;
    float exponentation = 3.3f;
    return pow(fBm_noise(uv / scale, octaves, amplitude), exponentation);
}


 // Based on this video https://www.youtube.com/watch?v=GgW3MVOP8_A&t=108s
vec3 calcNormal() {
    // If we were sampling a texture, we would want 1 pixel, but since we arent, it's just a small UV
    float eps = 0.75;
    float left = sampleNoise(tessEvalUV + vec2(-eps, 0.0));
    float right = sampleNoise(tessEvalUV + vec2(eps, 0.0));
    float up = sampleNoise(tessEvalUV + vec2(0.0, eps));
    float down = sampleNoise(tessEvalUV + vec2(0.0, -eps));

    // https://stackoverflow.com/questions/49640250/calculate-normals-from-heightmap
    // But not with sampling
    return normalize(vec3(
        (left - right) * HEIGHT_AMPLITUDE,
        2.0 * eps,
        (down - up) * HEIGHT_AMPLITUDE
    ));
}

void main() {    
     
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec2 uv0 = tessControlUV[0];
    vec2 uv1 = tessControlUV[1];
    vec2 uv2 = tessControlUV[2];
    vec2 uv3 = tessControlUV[3];
    
    vec2 leftUV = uv0 + v * (uv3 - uv0);
    vec2 rightUV = uv1 + v * (uv2 - uv1);
    tessEvalUV = mix(leftUV, rightUV, u);

    vec4 pos0 = gl_in[0].gl_Position;
    vec4 pos1 = gl_in[1].gl_Position;
    vec4 pos2 = gl_in[2].gl_Position;
    vec4 pos3 = gl_in[3].gl_Position;

    // Calculate new position
    vec4 leftPos = pos0 + v * (pos3 - pos0);
    vec4 rightPos = pos1 + v * (pos2 - pos1);
    vec4 pos = mix(leftPos, rightPos, u);

    // TODO: Could generate the noise once on the CPU and then sample it here so it's cheaper
    float h = sampleNoise(tessEvalUV);
    pos.y += h * HEIGHT_AMPLITUDE;

    // Calculate new normals
    terrainNormal = mat3(viewMatrix * modelMatrix) * calcNormal(); 
    // Calculate position for lighting
    viewPos = vec3(viewMatrix * modelMatrix * pos).xyz; 

    // Calculate actual position in screen-space
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * pos;
}
