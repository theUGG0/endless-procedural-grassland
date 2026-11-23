#include "shader_util.h"
#include "Camera.h"
#include "TerrainChunk.h"

#include <iostream>
#include <stack>
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/gtx/hash.hpp>
#include <glm/glm.hpp>

#include <unordered_map>

using namespace glm;

// Window Constants
const float SCREEN_WIDTH = 640;
const float SCREEN_HEIGHT = 480;

// Terrain variables
bool SHOW_TERRAIN_POLYGONS = false;
const int CHUNK_VIEW_DISTANCE = 12;
const int SIZE_OF_CHUNK = 100.0;

// Load shaders in
#define GLSL(src) #src
#define CONCAT_PATHS(A, B) A "/" B
#define ADD_ROOT(B) CONCAT_PATHS(PROJ_ROOT_PATH, B)

//----------------- Shaders ----------------//
shader_prog shader(ADD_ROOT("shaders/basic.vert.glsl"),
                   ADD_ROOT("shaders/basic.frag.glsl"));
shader_prog terrainShader(
    ADD_ROOT("shaders/terrain.vert"),
    ADD_ROOT("shaders/terrain.frag"),
    ADD_ROOT("shaders/terrain.tesc"),
    ADD_ROOT("shaders/terrain.tese")
);

std::unordered_map<glm::ivec2, GLuint> chunkVAOMap; // Store Chunks by X and Y chunk coords
int chunkIndexCount; // Stores index count for 1 chunk

Camera mainCamera(
    perspective(radians(90.0f), SCREEN_WIDTH/SCREEN_HEIGHT, 0.1f, 3000.0f), // Projection
    vec3(0.0, 10.0, 0.0) // Position
);

static void error_callback(int error, const char *description) {
  fputs(description, stderr);
}

// NOTE: Should move these to some inputManager class if we want it out of main.cpp
// ---------- Input Related Functions -----------//
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  else if (key == GLFW_KEY_T && action == GLFW_PRESS)
    SHOW_TERRAIN_POLYGONS = !SHOW_TERRAIN_POLYGONS; // toggle tessellation showcase
}

double mousePosX = 0.0;
double mousePosY = 0.0;
float sensitivity = 0.2;
// Process Mouse movement
static void cursor_callback(GLFWwindow *window, double new_pos_x, double new_pos_y) {

  // Look around with camera based on mouse movement
  float mouse_dx = new_pos_x - mousePosX;
  float mouse_dy = new_pos_y - mousePosY;

  mainCamera.rotateCamera(mouse_dx * sensitivity, -mouse_dy * sensitivity);

  // Update mouse pos variables
  mousePosX = new_pos_x;
  mousePosY = new_pos_y;
}

// Process input with delta time. Using OS KeyCallbacks behaves
// like typing so we dont want that for movement
static void processInput(GLFWwindow* win, float delta_time) {
  float moveSpeed = 10000.0f * delta_time;
  if (glfwGetKey(win, GLFW_KEY_LEFT_SHIFT)) {
    moveSpeed = 60.0f * delta_time;
  }
  if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) {
    mainCamera.move(glm::vec3(0.0, 0.0, -1.0 * moveSpeed));
  }
  if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) {
    mainCamera.move(glm::vec3(0.0, 0.0, 1.0 * moveSpeed));
  }
  if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) {
    mainCamera.move(glm::vec3(-1.0 * moveSpeed, 0.0, 0.0));
  }
  if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) {
    mainCamera.move(glm::vec3(1.0 * moveSpeed, 0.0, 0.0));
  }
}

// NOTE: Remove this afterwards
// create a chunk for testing
GLuint createChunk(glm::vec3 pos) {
  TerrainChunk testChunk {pos, SIZE_OF_CHUNK, 20};
  chunkIndexCount = testChunk.indices.size();
  
  GLuint vertexArrayHandle;
  glGenVertexArrays(1, &vertexArrayHandle);
  glBindVertexArray(vertexArrayHandle);

  // Store per vertex data and send it to the shader
  terrainShader.attribute3fv("position", &testChunk.vertices.position[0][0], testChunk.vertices.position.size() * 3);
  
  // Store index data in the buffer
  GLuint vboHandle;
  glGenBuffers(1, &vboHandle);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboHandle);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, testChunk.indices.size() * sizeof(testChunk.indices[0]), testChunk.indices.data(), GL_STATIC_DRAW);
  
  return vertexArrayHandle;
}

void drawChunk(glm::ivec2 chunk_pos, bool showcase_tessellation = false) {
    std::stack<mat4> ms;
    ms.push(mat4(1.0));

    // Cube stack
    ms.push(ms.top());
      ms.top() = glm::translate(ms.top(), vec3(0.0, 0.0, -5.0));
      terrainShader.uniformMatrix4fv("modelMatrix", ms.top());
      
      // ENABLE/DISABLE Tessellation showcase
      if (showcase_tessellation) {
        terrainShader.uniform3f("lineColor", 0.0, 1.0, 0.0);
      } else {
        terrainShader.uniform3f("lineColor", 0.0, 0.0, 0.0);  
      }
      
      glBindVertexArray(chunkVAOMap[chunk_pos]);
      glDrawElements(GL_PATCHES, chunkIndexCount, GL_UNSIGNED_INT, 0);
    ms.pop();
}

static void render(GLFWwindow *window) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glm::vec3 camPos = mainCamera.getPosition();

  
  // Render generated chunks
  for (int chunkX = -CHUNK_VIEW_DISTANCE; chunkX <= CHUNK_VIEW_DISTANCE; chunkX++) {
      for (int chunkZ = -CHUNK_VIEW_DISTANCE; chunkZ <= CHUNK_VIEW_DISTANCE; chunkZ++) {
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0f, 1.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glm::ivec2 cameraChunkCoords = glm::vec2(static_cast<int>(camPos.x / SIZE_OF_CHUNK), static_cast<int>(camPos.z / SIZE_OF_CHUNK));        
        glm::ivec2 chunkPos = glm::ivec2(chunkX + cameraChunkCoords[0], chunkZ + cameraChunkCoords[1]);
        
        
        drawChunk(chunkPos);  
        glDisable(GL_POLYGON_OFFSET_FILL);
        if (SHOW_TERRAIN_POLYGONS) {
          glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
          drawChunk(chunkPos, SHOW_TERRAIN_POLYGONS);
        }
    }
  }
}

// Generates AMOUNT OF CHUNKS from camera pos
void updateChunks() {
    glm::vec3 camPos = mainCamera.getPosition();
    glm::ivec2 cameraChunkCoords = glm::vec2(floor(camPos.x / SIZE_OF_CHUNK), floor(camPos.z / SIZE_OF_CHUNK));

    for (int chunkX = -CHUNK_VIEW_DISTANCE; chunkX <= CHUNK_VIEW_DISTANCE; chunkX++) {
        for (int chunkZ = -CHUNK_VIEW_DISTANCE; chunkZ <= CHUNK_VIEW_DISTANCE; chunkZ++) {
          
            glm::ivec2 chunkPos = cameraChunkCoords + glm::ivec2(chunkX, chunkZ);

            // If chunk does not exist, create it
            if (chunkVAOMap.find(chunkPos) == chunkVAOMap.end()) {
                glm::ivec2 chunkWorldPos = glm::vec2(chunkPos[0] * SIZE_OF_CHUNK, chunkPos[1] * SIZE_OF_CHUNK);
                chunkVAOMap[chunkPos] = createChunk(glm::vec3(chunkWorldPos[0], 0.0f, chunkWorldPos[1]));
            }            
        }
    }
    
    // Loop over each chunk and check if it still needs to exist
    for (auto it = chunkVAOMap.begin(); it != chunkVAOMap.end();) {
        glm::ivec2 chunkPos = it->first;
        GLuint chunkVAO = it->second;
        glm::ivec2 chunkDistance = glm::abs(cameraChunkCoords - chunkPos);
        if ( chunkDistance[0] > (CHUNK_VIEW_DISTANCE + 1) || chunkDistance[1] > (CHUNK_VIEW_DISTANCE + 1)) {
            glDeleteVertexArrays(1, &chunkVAO);
            it = chunkVAOMap.erase(it); // If something was deleted, keep going from there
        } else { ++it; } // If nothing was deleted keep going as normal
    }

}

int main(void) {
  GLFWwindow *window;

  glfwSetErrorCallback(error_callback);

  if (!glfwInit())
    exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Grasslands", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  // GLFW setup
  glfwSwapInterval(1); // Vsync
  glfwMakeContextCurrent(window);
  glewExperimental = GL_TRUE;
  glewInit();

  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, cursor_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Captures cursor

  // OpenGL settings
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glClearColor(0.38f, 0.667f, 1.0f, 1.0f); // Background color is a nice blue

  terrainShader.use();
  glPatchParameteri(GL_PATCH_VERTICES, 4);
  
  // Init scene (test chunks)
  updateChunks();


  float lastFrameTime = 0.0f;
  while (!glfwWindowShouldClose(window)) {
    
    // Delta Time for frames
    float currentFrame = static_cast<float>(glfwGetTime());
    float deltaTime = currentFrame - lastFrameTime;
    lastFrameTime = currentFrame;
    std::cout << "FPS:" << (1.0f / deltaTime) << '\n'; 

    processInput(window, deltaTime);    

    // Chunk generation delete/generate logic
    updateChunks();
    
    // Give the shader our camera data before rendering
    terrainShader.uniformMatrix4fv("projectionMatrix", mainCamera.getProjection());
    terrainShader.uniformMatrix4fv("viewMatrix", mainCamera.getView());
    

    render(window);

    glfwSwapBuffers(window);
    glfwPollEvents();
    // glfwWaitEvents();
  }

  glfwDestroyWindow(window);

  glfwTerminate();
  exit(EXIT_SUCCESS);
}
