#version 460 core

// Based on this video
// https://www.youtube.com/watch?v=21gfE-zUym8&t

layout (vertices = 4) out;

in vec2 uv[];
out vec2 uvCoord[];

uniform mat4 viewMatrix;
vec4 cameraPos = vec4(viewMatrix[0][3], viewMatrix[1][3], viewMatrix[2][3], 1.0);

const int MIN_TES = 1;  // 0 makes them disappear
const int MAX_TES = 6; 
const float MIN_DIST = 0.0;
const float MAX_DIST = 30.0;

void main() {
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    uvCoord[gl_InvocationID] = uv[gl_InvocationID];

    if (gl_InvocationID == 0) {

        // Find the centers of each side of the patch
        vec4 center0 = gl_in[0].gl_Position + (gl_in[3].gl_Position - gl_in[0].gl_Position) / 2.0; // left side
        vec4 center1 = gl_in[1].gl_Position + (gl_in[0].gl_Position - gl_in[1].gl_Position) / 2.0; // bot side
        vec4 center2 = gl_in[2].gl_Position + (gl_in[1].gl_Position - gl_in[2].gl_Position) / 2.0; // right side
        vec4 center3 = gl_in[3].gl_Position + (gl_in[2].gl_Position - gl_in[3].gl_Position) / 2.0; // top side

        // Find distance from camera
        float dist0 = length(cameraPos - center0);
        float dist1 = length(cameraPos - center1);
        float dist2 = length(cameraPos - center2);
        float dist3 = length(cameraPos - center3);

        // Interpolate between Tessellation levels based on distance
        int tes0 = int(mix(MAX_TES, MIN_TES, clamp(dist0 / MAX_DIST, 0.0, 1.0)));
        int tes1 = int(mix(MAX_TES, MIN_TES, clamp(dist1 / MAX_DIST, 0.0, 1.0)));
        int tes2 = int(mix(MAX_TES, MIN_TES, clamp(dist2 / MAX_DIST, 0.0, 1.0)));
        int tes3 = int(mix(MAX_TES, MIN_TES, clamp(dist3 / MAX_DIST, 0.0, 1.0)));
        
        gl_TessLevelOuter[0] = tes0; // Left for quads
        gl_TessLevelOuter[1] = tes1; // Bot for quads
        gl_TessLevelOuter[2] = tes2; // Right for quads
        gl_TessLevelOuter[3] = tes3; // Top for quads

        gl_TessLevelInner[0] = max(tes1, tes3); // Up down for quads
        gl_TessLevelInner[1] = max(tes0, tes2); // Left right for quads
    }
}
