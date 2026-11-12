
uint PCGHash(uint rng_state)
{
    // gotten from https://www.shadertoy.com/view/ctj3Wc
    rng_state = rng_state * 747796405u + 2891336453u;
    uint state = rng_state;
    uint word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
    return (word >> 22u) ^ word;
}

float coordinate_hash(ivec2 coords) {

    uint hash_uint = PCGHash(PCGHash(uint(coords.x))^uint(coords.y));

    // make it a value between 0...1
    return float(hash_uint) / 4294967295.0;
}

float value_noise(vec2 coords) {
    // gotten from https://thebookofshaders.com/11/

    ivec2 fl = ivec2(floor(coords));
    
    // corners we interpolate between
    float a = coordinate_hash(fl);
    float b = coordinate_hash(fl + ivec2(1, 0));
    float c = coordinate_hash(fl + ivec2(0, 1));
    float d = coordinate_hash(fl + ivec2(1, 1));

    // cubic Hermite curve
    vec2 fr = fract(coords);

    vec2 hermite = fr * fr * (3.0 - 2.0 * fr); 

    // get the interpolated values, applying Hermite interpolation
    float noise = mix(a, b, hermite.x) + (c - a) * hermite.y * (1.0 - hermite.x) + (d - b) * hermite.x * hermite.y;

    return noise;
}

float fBm_noise (vec2 coords, int octaves, float amplitude){
    
    float value = 0.0;
    float amp_sum = 0.0;

    for (int i = 0; i < octaves; i++){
        value += amplitude * value_noise(coords);
        coords *= 2.0;
        amp_sum += amplitude;
        amplitude *= 0.5;
    }

    return value / amp_sum;
}