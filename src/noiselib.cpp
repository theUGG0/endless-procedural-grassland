#include <glm/glm.hpp>
#include <cstdint>

uint32_t PCGHash(uint32_t rng_state)
{
    rng_state = rng_state * 747796405u + 2891336453u;
    uint32_t state = rng_state;
    uint32_t word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
    return (word >> 22u) ^ word;
}


float coordinate_hash(int x, int y) {

    uint32_t hash_uint = PCGHash(PCGHash(x)^y );

    return static_cast<float>(hash_uint) / UINT32_MAX;
}