#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <vector>

// NOTE: If multiple things need this, then move to types.h
struct VertexData {
  std::vector<glm::vec3> position;
  std::vector<glm::vec2> uv;
};

class TerrainChunk {
public:
glm::vec3 position;
VertexData vertices;
std::vector<GLuint> indices;

// Consturct a chunk plane with a center position, size and amount of divisions
TerrainChunk(glm::vec3 origin_pos, float size, int divs);
void render() {
    
}
  
private:

// NOTE: This can and probably needs to be moved to a compute shader later on.
// Generation time would be like a million times faster
//---------- Init Functions ----------//

// Based on this videos code https://www.youtube.com/watch?v=FKLbihqDLsg
// Generate the positions and UV for the vertices of the plane
void initializeVertexData(float size, int divs);

// Generate the indices for the vertices of the plane
void initializeIndices(int divs);
};
