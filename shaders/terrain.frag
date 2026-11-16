#version 330

#include "noise.glsl"

// This is the output variable of the fragment shader
// We configure this to be "output 0" for our fragment shader
// in the main program using glBindFragDataLocation

out vec4 FragColor;
in vec2 tessEvalUV;
in float teHeight;

void main(void) {
    float noise = fBm_noise(tessEvalUV * 0.1, 7, 2.5);
    FragColor = vec4(vec3(noise), 1.0);
}
