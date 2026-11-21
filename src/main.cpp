#include "shader_util.h"
#include "Camera.h"

#include <cmath>
#include <stack>
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace glm;

const float SCREEN_WIDTH = 640;
const float SCREEN_HEIGHT = 480;

//---------- Forward Declarations ----------//
void drawCube();
// void drawGrassBlade(float x, float y, float z);
void drawGrassBlade();

// Load shaders in
#define GLSL(src) #src
#define CONCAT_PATHS(A, B) A "/" B
#define ADD_ROOT(B) CONCAT_PATHS(PROJ_ROOT_PATH, B)
shader_prog shader(ADD_ROOT("shaders/basic.vert.glsl"),
                   ADD_ROOT("shaders/basic.frag.glsl"));
shader_prog grass_shader(ADD_ROOT("shaders/grass.vert.glsl"),
                   ADD_ROOT("shaders/grass.frag.glsl"));
GLuint cubeVAO;
GLuint grassBladeVAO;
GLuint instanceVBO;

Camera mainCamera(
    perspective(radians(120.0f), SCREEN_WIDTH/SCREEN_HEIGHT, 0.1f, 100.0f), // Projection
    vec3(0.0, 0.0, 0.0) // Position
);

static void error_callback(int error, const char *description) {
  fputs(description, stderr);
}

// NOTE: Should move these to some inputManager class if we want it out of main.cpp
// ---------- Input Related Functions -----------//
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
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

// Process input with delta time. Using OS KeyCallbacks behaves like typing
// which is not ideal for movement, as pressing a key would wait a bit before it starts repeating
static void processInput(GLFWwindow* win, float deltaTime) {
  float moveSpeed = 0.1f;
  if (glfwGetKey(win, GLFW_KEY_LEFT_SHIFT)) {
    moveSpeed = 2.0f;
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

static void render(GLFWwindow *window) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // Render a simple cube for testing
  // drawCube();  
  glUseProgram(grass_shader);
  drawGrassBlade();
}


// creating equilateral triangle so it should look better in larger quantities
void createGrassInstances() {
  // random offsets for now
  // TODO: make them appear random i guess and make them procedurally generate
  const GLfloat offsets[] = {
    -3.0f, 0.0f, -5.0f,
    0.0f, 0.0f, -5.0f,
    3.0f, 0.0f, -5.0f,
    -1.5f, 0.0f, -8.0f,
    1.5f, 0.0f, -8.0f
  };

  glBindVertexArray(grassBladeVAO);

  glGenBuffers(1, &instanceVBO);
  glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(offsets), offsets, GL_STATIC_DRAW);

  glEnableVertexAttribArray(1);//enable instanceOffset
  glVertexAttribPointer(1,3,GL_FLOAT, GL_FALSE, 3* sizeof(GLfloat), nullptr);// describing how to read attribute
  glVertexAttribDivisor(1,1);//advance once per blade

  glBindVertexArray(0);// unbind VAO

}

GLuint createGrassBlade() {
  float s = 2.0;

  float h = (std::sqrt(3.0f)*s)/2.0f;

  GLfloat vertices[] = {
      0.0f, 1.0f, 0.0f,
      0.0f, -1.0f, 0.0f,
      0.33f, 1.0f, 0.0f,
      0.33f, -1.0f, 0.0f,
      0.66f, 1.0f, 0.0f,
      0.66f, -1.0f, 0.0f,
      1.0f, 0.0f, 0.0f
  };

  GLubyte indices[] = {
    // front
    0,1,2,
    2,1,3,
    2,3,4,
    4,3,5,
    4,5,6,
    // back
    0,2,1,
    2,3,1,
    2,4,3,
    4,5,3,
    4,6,5
  };

  GLuint vertexArrayHandle;
  glGenVertexArrays(1, &vertexArrayHandle);
  glBindVertexArray(vertexArrayHandle);

  // Store per vertex data and send it to the shader
  grass_shader.attribute3fv("position", vertices, 21);
  
  // Store index data in the buffer
  GLuint vboHandle;
  glGenBuffers(1, &vboHandle);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboHandle);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  
  return vertexArrayHandle;
};

// NOTE: Remove this afterwards
// create a cube for testing
GLuint createCube() {
  float s = 2.0;

  GLfloat vertices[] = {
      -s, -s,  s,
       s, -s,  s,
       s,  s,  s,
      -s,  s,  s,
      -s, -s, -s,
       s, -s, -s,
       s,  s, -s,
      -s,  s, -s,
  };

  GLubyte indices[] = {
      // Front
      0, 1, 2,
      0, 2, 3,

      // Back
      4, 7, 6,
      4, 6, 5,

      // Bottom
      4, 5, 1,
      4, 1, 0,

      // Top
      3, 2, 6,
      3, 6, 7,

      // Left
      4, 0, 3,
      4, 3, 7,

      // Right
      1, 5, 6,
      1, 6, 2,
  };


  

  GLuint vertexArrayHandle;
  glGenVertexArrays(1, &vertexArrayHandle);
  glBindVertexArray(vertexArrayHandle);

  // Store per vertex data and send it to the shader
  shader.attribute3fv("position", vertices, 24);

  // Store index data in the buffer
  GLuint vboHandle;
  glGenBuffers(1, &vboHandle);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboHandle);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  return vertexArrayHandle;
}

void drawCube() {
  std::stack<mat4> ms;
  ms.push(mat4(1.0));

  // Cube stack
  ms.push(ms.top());
    ms.top() = glm::translate(ms.top(), vec3(0.0, 0.0, -5.0));
    shader.uniformMatrix4fv("modelMatrix", ms.top());
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, 0);
  ms.pop();
}

void drawGrassBlade() {
    grass_shader.uniformMatrix4fv("modelMatrix", mat4(1.0f));
    glBindVertexArray(grassBladeVAO);
    glDrawElementsInstanced(GL_TRIANGLES, 30, GL_UNSIGNED_BYTE, nullptr, 5);// 5 is number of blades
}

int main(void) {
  GLFWwindow *window;

  glfwSetErrorCallback(error_callback);

  if (!glfwInit())
    exit(EXIT_FAILURE);


  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Grasslands", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);
  glewExperimental = GL_TRUE;
  glewInit();

  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, cursor_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Captures cursor

  grass_shader.use();
  shader.use();

  // Init scene (test cube)
  // cubeVAO = createCube();
  grassBladeVAO = createGrassBlade();
  // creates insctances
  createGrassInstances();

  float lastFrameTime = 0.0f;
  while (!glfwWindowShouldClose(window)) {
    
    // Delta Time for frames
    float currentFrame = static_cast<float>(glfwGetTime());
    float deltaTime = currentFrame - lastFrameTime;

    processInput(window, deltaTime);
    
    // Give the shader our camera data before rendering
    glUseProgram(shader);
    shader.uniformMatrix4fv("projectionMatrix", mainCamera.getProjection());
    shader.uniformMatrix4fv("viewMatrix", mainCamera.getView());
    glUseProgram(grass_shader);
    grass_shader.uniformMatrix4fv("projectionMatrix", mainCamera.getProjection());
    grass_shader.uniformMatrix4fv("viewMatrix", mainCamera.getView());
    render(window);

    glfwSwapBuffers(window);
    glfwPollEvents();
    // glfwWaitEvents();
  }

  glfwDestroyWindow(window);

  glfwTerminate();
  exit(EXIT_SUCCESS);
}
