#include <glm/common.hpp>
#include <glm/ext/quaternion_common.hpp>
#include <glm/glm.hpp>
#include <cstdint>

uint32_t PCGHash(uint32_t rng_state)
{
    // gotten from https://www.shadertoy.com/view/ctj3Wc
    rng_state = rng_state * 747796405u + 2891336453u;
    uint32_t state = rng_state;
    uint32_t word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
    return (word >> 22u) ^ word;
}

float coordinate_hash(int x, int y) {

    // XOR for combining hashes as per https://stackoverflow.com/questions/5889238/why-is-xor-the-default-way-to-combine-hashes
    uint32_t hash_uint = PCGHash(PCGHash(x)^y );

    // make it a value between 0...1
    return static_cast<float>(hash_uint) / UINT32_MAX;
}

float value_noise(float x, float y) {
    // gotten from https://thebookofshaders.com/11/

    int x_fl = glm::floor(x);
    int y_fl = glm::floor(y);
    
    // corners we interpolate between
    float a = coordinate_hash(x_fl, y_fl);
    float b = coordinate_hash(x_fl + 1, y_fl);
    float c = coordinate_hash(x_fl, y_fl + 1);
    float d = coordinate_hash(x_fl + 1, y_fl + 1);

    // cubic Hermine curve
    float x_fr = glm::fract(x);
    float y_fr = glm::fract(y);

    float hermite_x = x_fr * x_fr * (3.0f - 2.0f * x_fr); 
    float hermite_y = y_fr * y_fr * (3.0f - 2.0f * y_fr); 

    // get the interpolated values, applying Hermite interpolation
    float noise = glm::mix(a, b, hermite_x) + (c - a) * hermite_y * (1.0f - hermite_x) + (d - b) * hermite_x * hermite_y;

    return noise;
}

float fBm_noise (float x, float y, int octaves, float amplitude){
    
    float value = 0.0f;

    for (int i = 0; i < octaves; i++){
        value += amplitude * value_noise(x, y);
        x *= 2;
        y *= 2;
        amplitude *= 0.5;
    }

    return value;
}