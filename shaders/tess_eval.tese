#version 460 core

// Based on this video
// https://www.youtube.com/watch?v=21gfE-zUym8&t

layout (quads, equal_spacing, ccw) in;

in vec2 uvCoord[];
out vec2 uvs;

void main() {
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec2 uv0 = uvCoord[0];
    vec2 uv1 = uvCoord[1];
    vec2 uv2 = uvCoord[2];
    vec2 uv3 = uvCoord[3];
    
    vec2 leftUV = uv0 + v * (uv3 - uv0);
    vec2 rightUV = uv1 + v * (uv2 - uv1);
    // Bilinear interpolation
    vec2 texCoord = leftUV + u * (rightUV - leftUV);

    vec4 pos0 = gl_in[0].gl_Position;
    vec4 pos1 = gl_in[1].gl_Position;
    vec4 pos2 = gl_in[2].gl_Position;
    vec4 pos3 = gl_in[3].gl_Position;

    vec4 leftPos = pos0 + v * (pos3 - pos0);
    vec4 rightPos = pos1 + v * (pos2 - pos1);
    // Bilinear interpolation
    vec4 pos = leftPos + u * (rightPos - leftPos);

    gl_Position = pos; // Matrix transforms here
    uvs = texCoord;
}
