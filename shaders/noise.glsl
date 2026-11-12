
uint PCGHash(uint rng_state)
{
    // gotten from https://www.shadertoy.com/view/ctj3Wc
    rng_state = rng_state * 747796405u + 2891336453u;
    uint state = rng_state;
    uint word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
    return (word >> 22u) ^ word;
}

float coordinate_hash(int x, int y) {

    uint hash_uint = PCGHash(uint(PCGHash(x))^uint(y));

    // make it a value between 0...1
    return float(hash_uint) / 4294967295.0;
}