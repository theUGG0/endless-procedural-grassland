#version 330

#include "noise.glsl"

// This is the output variable of the fragment shader
// We configure this to be "output 0" for our fragment shader
// in the main program using glBindFragDataLocation

out vec4 fragColor;

void main(void) {
    float noise = fBm_noise(gl_FragCoord.xy / 64.0, 7, 2.5);
    fragColor = vec4(vec3(noise), 1.0);
}
