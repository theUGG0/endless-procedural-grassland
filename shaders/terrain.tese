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
out float teHeight;

void main() {
    // TODO: HARDCODED DIRECTIONAL LIGHT
    vec3 lightPosition = vec3(0.25, -0.5, 0.25);
    
     
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

    vec4 leftPos = pos0 + v * (pos3 - pos0);
    vec4 rightPos = pos1 + v * (pos2 - pos1);
    vec4 pos = mix(leftPos, rightPos, u);

    // TODO: Do this on the CPU and then just sample the passed in texture so it's cheaper
    float sizeOfChunk = 20.0f; // This name is incorrect
    float amplitude = 2.0f;
    int octaves = 4;
    float exponentation = 5.3f;
    float heightAmplitude = 20.0f;
    float h = pow(fBm_noise(tessEvalUV / sizeOfChunk, octaves, amplitude), exponentation);
    pos.y += h * heightAmplitude;
    teHeight = h;
    
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * pos;
}
