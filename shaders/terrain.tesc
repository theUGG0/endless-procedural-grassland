#version 460 core

// Based on this video
// https://www.youtube.com/watch?v=21gfE-zUym8&t

layout (vertices = 4) out;

uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

const int MIN_TES = 1;  // 0 makes them disappear
const int MAX_TES = 8;  // I dont think we need more than 8
const float MIN_DIST = 1.0;
const float MAX_DIST = 100.0;

void main() {
    mat4 modelView = viewMatrix * modelMatrix;
    
    // Get the position of the vertices in the control patch
    // The vertex shader was run 4 times before, so now we get each result via index
    // This is because CPU side, we did glPatchParameteri(GL_PATCH_VERTICES, 4)
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    if (gl_InvocationID == 0) {

        // Get all the vertices into view-space
        vec4 p0 = modelView * gl_in[0].gl_Position;
        vec4 p1 = modelView * gl_in[1].gl_Position;
        vec4 p2 = modelView * gl_in[2].gl_Position;
        vec4 p3 = modelView * gl_in[3].gl_Position;

        // Calculate the distance from the camera to the vertices
        float len0 = length(p0.xyz); // bottom left
        float len1 = length(p1.xyz); // bottom right
        float len2 = length(p2.xyz); // top right
        float len3 = length(p3.xyz); // top left

        // Map to [0, 1]
        float dist0 = clamp((len0 - MIN_DIST) / (MAX_DIST - MIN_DIST), 0.0, 1.0);
        float dist1 = clamp((len1 - MIN_DIST) / (MAX_DIST - MIN_DIST), 0.0, 1.0);
        float dist2 = clamp((len2 - MIN_DIST) / (MAX_DIST - MIN_DIST), 0.0, 1.0);
        float dist3 = clamp((len3 - MIN_DIST) / (MAX_DIST - MIN_DIST), 0.0, 1.0);
        
        // Interpolate between Tessellation levels based on distance
        float tes0 = mix(MAX_TES, MIN_TES, min(dist0, dist3));
        float tes1 = mix(MAX_TES, MIN_TES, min(dist0, dist1));
        float tes2 = mix(MAX_TES, MIN_TES, min(dist1, dist2));
        float tes3 = mix(MAX_TES, MIN_TES, min(dist3, dist2));

        // Set outer tessellation levels
        gl_TessLevelOuter[0] = tes0; // Left for quads
        gl_TessLevelOuter[1] = tes1; // Bot for quads
        gl_TessLevelOuter[2] = tes2; // Right for quads
        gl_TessLevelOuter[3] = tes3; // Top for quads

        // Set inner tessellation levels
        gl_TessLevelInner[0] = max(tes1, tes3); // Up down for quads
        gl_TessLevelInner[1] = max(tes0, tes2); // Left right for quads
    }
}
