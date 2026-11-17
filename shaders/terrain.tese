#version 460 core

#include "noise.glsl"

// Based on this video
// https://www.youtube.com/watch?v=21gfE-zUym8&t

layout (quads, fractional_odd_spacing, ccw) in;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out vec2 noiseCoord;
out vec3 terrainPos;
out vec3 terrainNormal;

const float HEIGHT_AMPLITUDE = 80.0;

float sampleNoise(vec2 uv) {
    float scale = 120.0f;       // Basically how subtle the mountains are: higher -> smoother 
    float amplitude = 0.5f;     // I didnt feel a practical difference in my testing
    int octaves = 5;            // How bumpy the terrain is
    float exponentiation = 4.0f; // How flat the terrain is
    return pow(fBm_noise(uv / scale, octaves, amplitude), exponentiation);
}


vec3 calcNormal(vec2 origin_coord) {
    // Based on this video https://youtu.be/GgW3MVOP8_A?si=Qz2E4Jkb48Zm74_F&t=555
    // If we were sampling a texture, we would want 1 pixel, but since we arent, it's just a small UV
    float eps = 0.75;
    float left = sampleNoise(origin_coord + vec2(-eps, 0.0));
    float right = sampleNoise(origin_coord + vec2(eps, 0.0));
    float up = sampleNoise(origin_coord + vec2(0.0, eps));
    float down = sampleNoise(origin_coord + vec2(0.0, -eps));

    // Similar to this https://stackoverflow.com/questions/49640250/calculate-normals-from-heightmap
    return normalize(vec3((left - right) * HEIGHT_AMPLITUDE, 2.0 * eps, (down - up) * HEIGHT_AMPLITUDE));
}

void main() {    
    mat4 modelView = viewMatrix * modelMatrix;  
    
    // Get this vertices position in the control patch
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    // Get the position of the vertices in the control patch passed from control shader
    vec4 bl_pos = gl_in[0].gl_Position; // model coords of the bottom left corner
    vec4 br_pos = gl_in[1].gl_Position; // model coords of the bottom right corner
    vec4 tr_pos = gl_in[2].gl_Position; // model coords of the top right corner
    vec4 tl_pos = gl_in[3].gl_Position; // model coords of the top left corner

    // Calculate new position and world coords (which will be used to sample noise)
    // via bilinear interpolation inside the bounds of the control patch
    vec4 pos   = mix(mix(bl_pos, tl_pos, v), mix(br_pos, tr_pos, v), u);
    noiseCoord = (modelMatrix * pos).xz;

    // TODO: Could generate the noise once on the CPU and then sample it here so it's cheaper
    // This would probably really help when calculating normals, as those need 4 samples, so doing all that math is probably way slower
    float h = sampleNoise(noiseCoord);
    pos.y += h * HEIGHT_AMPLITUDE;

    // Calculate new normals and pos in view-space. This way i dont have to pass in cameraPos
    // as cameraPos is already encoded in the viewMatrix
    terrainNormal = mat3(modelView) * calcNormal(noiseCoord); 
    terrainPos = vec3(modelView * pos).xyz; 

    // Calculate actual position in screen-space
    gl_Position = projectionMatrix * modelView * pos;
}
