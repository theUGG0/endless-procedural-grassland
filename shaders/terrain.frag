#version 330

#include "noise.glsl"


uniform vec3 lineColor;

out vec4 FragColor;
in vec2 tessEvalUV;
in float teHeight;

// // Based on this video https://www.youtube.com/watch?v=GgW3MVOP8_A&t=108s
// vec3 calcNormal() {
//     float left = textureOffset(noiseTexture, tessEvalUV, ivec2(-1, 0)).r;
//     float right = textureOffset(noiseTexture, tessEvalUV, ivec2(1, 0)).r;
//     float up = textureOffset(noiseTexture, tessEvalUV, ivec2(0, 1)).r;
//     float down = textureOffset(noiseTexture, tessEvalUV, ivec2(0, -1)).r;

//     // https://stackoverflow.com/questions/49640250/calculate-normals-from-heightmap
//     vec3 normal = normalize(vec3(right - left, up - down, -2.0));
// }

void main(void) {
    float noise = fBm_noise(tessEvalUV * 0.1, 4, 2.0);
    FragColor = vec4(vec3(noise), 1.0);

    if (lineColor != vec3(0.0)) {
        FragColor = vec4(vec3(lineColor), 1.0);
    }
}
