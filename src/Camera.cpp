#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void Camera::lookAt(glm::vec3 pos, glm::vec3 target, glm::vec3 up) {
  position = pos;
  view = glm::lookAt(pos, target, up);
}


// NOTE: This could probably be optimized as im recalculating front all the time
// Which uses cos. Probably a better solution
// Move the camera in local XYZ
void Camera::move(glm::vec3 move_by) { 
  // Find all relative directions
  glm::vec3 front =
    normalize(
      glm::vec3(
        cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
        sin(glm::radians(pitch)),
        sin(glm::radians(yaw)) * cos(glm::radians(pitch))
      )
    );
    
  glm::vec3 right = normalize(cross(front, glm::vec3(0.0, 1.0, 0.0)));
  glm::vec3 up = normalize(cross(right, front));

  // Move in each axis after the rotation
  position += right * move_by.x;
  position += up * move_by.y;
  position -= front * move_by.z;

  // Always look in front, not specifically AT some place
  view = glm::lookAt(position, position + front, glm::vec3(0.0, 1.0, 0.0));
}

// Calculate the rotation of the camera via pitch and yaw
void Camera::rotateCamera(float by_yaw, float by_pitch) {
  // Update the internal variables
  yaw += by_yaw;
  pitch += by_pitch;

  glm::vec3 front =
    normalize(
      glm::vec3(
        cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
        sin(glm::radians(pitch)),
        sin(glm::radians(yaw)) * cos(glm::radians(pitch))
      )
    );
  lookAt(position, position + front, glm::vec3(0.0, 1.0, 0.0));
}
