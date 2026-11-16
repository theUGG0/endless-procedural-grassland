#version 460 core

// Based on this video
// https://www.youtube.com/watch?v=21gfE-zUym8&t

layout (vertices = 4) out;

in vec2 vertexUV[];
out vec2 tessControlUV[];

uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

const int MIN_TES = 1;  // 0 makes them disappear
const int MAX_TES = 16; 
const float MIN_DIST = 0.0;
const float MAX_DIST = 30.0;

float distToCamera(vec4 objPos) {
    return length((viewMatrix * modelMatrix * objPos).xyz);
}

void main() {
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    tessControlUV[gl_InvocationID] = vertexUV[gl_InvocationID];

    if (gl_InvocationID == 0) {

        vec4 p0 = gl_in[0].gl_Position;
        vec4 p1 = gl_in[1].gl_Position;
        vec4 p2 = gl_in[2].gl_Position;
        vec4 p3 = gl_in[3].gl_Position;

        // Edge midpoints
        float dist0 = distToCamera(mix(p0, p3, 0.5)); // left
        float dist1 = distToCamera(mix(p0, p1, 0.5)); // bottom
        float dist2 = distToCamera(mix(p1, p2, 0.5)); // right
        float dist3 = distToCamera(mix(p3, p2, 0.5)); // top

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
