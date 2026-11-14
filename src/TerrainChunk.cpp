#include "TerrainChunk.h"

#include <glm/glm.hpp>


TerrainChunk::TerrainChunk(glm::vec3 origin_pos, float size, int divs): position{origin_pos}  {
    initializeVertexData(size, divs);
    initializeIndices(divs);
}

// Based on this videos code https://www.youtube.com/watch?v=FKLbihqDLsg
// Generate the positions and UV for the vertices of the plane
void TerrainChunk::initializeVertexData(float size, int divs) {
    float triangleSide = size / divs;
    for (int row = 0; row < divs + 1; row++) {   
        for (int col = 0; col < divs + 1; col++) {
          vertices.position.emplace_back(col * triangleSide, 0.0, row * -triangleSide);
          vertices.uv.emplace_back((float(col) / divs), (float(row) / divs));
        }
    }
}

// Based on this videos code https://www.youtube.com/watch?v=21gfE-zUym8&t=93s
// Generate the indices for the vertices of the plane
void TerrainChunk::initializeIndices(int divs) {
    for (int row = 0; row < divs; row++) {
        for (int col = 0; col < divs; col++) {
            int bottomLeftIdxSqr = row * (divs + 1) + col;

            // Indices for square (a single patch for tessellation)
            indices.push_back(bottomLeftIdxSqr);                   
            indices.push_back(bottomLeftIdxSqr + 1);               
            indices.push_back(bottomLeftIdxSqr + (divs + 1) + 1);  
            indices.push_back(bottomLeftIdxSqr + (divs + 1));
        }
    }
}
