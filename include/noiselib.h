#include <glm/glm.hpp>

float coordinate_hash(int x, int y);

float value_noise(float x, float y);

float fBm_noise (float x, float y, int octaves, float amplitude);

/*
  // useful debugger for checking noise 
  std::ofstream pgm("img.pgm");
  pgm << "P2\n256 256\n256";

  for (int y = 0; y < 256; y++) {
    pgm << "\n";
    for (int x = 0; x < 256; x++) {
      pgm << static_cast<int>(256 * fBm_noise(x * 0.05, y * 0.05, 7, 0.5)) << " ";
    }
  }

  pgm.close();

*/