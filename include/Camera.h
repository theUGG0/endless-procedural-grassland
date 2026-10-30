#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Provides a convenient interface for interacting with the camera
class Camera {
  public:
    
  
  //--------- Constructors -----------//

  Camera(glm::mat4 in_projection, glm::vec3 in_pos = glm::vec3(0.0f)):
    projection(in_projection),
    position(in_pos)
    {
      view = glm::lookAt(in_pos, glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0));
    }
  
  //--------- Main Functionality -----------//
  
  // Wrapper for glm::lookAt, which also updates the internal position of the camera
  // Here incase we want to teleport the camera for some reason
  // Probably will not use it
  void lookAt(glm::vec3 pos, glm::vec3 target, glm::vec3 up);
  
  // Move camera by vector in local XYZ
  void move(glm::vec3 move_by);
  
  // Calculate the rotation of the camera via pitch and yaw
  void rotateCamera(float by_yaw, float by_pitch);
  
  //--------- Getter functions -----------//
  glm::mat4 getProjection() { return projection; } 
  glm::mat4 getView() { return view; } 
  glm::vec3 getPosition() { return position; } 
    
private:  
  glm::mat4 projection;
  glm::mat4 view;
  glm::vec3 position;

  float pitch = 0.0f;
  float yaw = 0.0f;
};
