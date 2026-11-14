/**
 * MTAT.03.015 Computer Graphics.
 * Shader configuration utility routines.
 */
#include <string>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

/**
 * Modified version of code from:
 *  http://stackoverflow.com/questions/2795044/easy-framework-for-opengl-shaders-in-c-c
 */
class shader_prog {
private:
    GLuint vertex_shader, fragment_shader, tess_control_shader, tess_eval_shader, prog;
    std::string v_source, f_source, tess_control_source, tess_eval_source;
public:
    shader_prog(const char* vertex_shader_filename, const char* fragment_shader_filename, const char* tessellation_shader_filename = NULL, const char* primitive_generator_shader_filename = NULL);
    void use();
    void free();
    operator GLuint();

    // Shorthands for glUniform specification
    void uniform1i(const char* name, int i);
    void uniform1f(const char* name, float f);
    void uniform2f(const char* name, float x, float y);
    void uniform3f(const char* name, float x, float y, float z);
    void uniformMatrix4fv(const char* name, const float* matrix);
    void uniformMatrix4fv(const char* name, glm::mat4 matrix);
    void attribute3fv(const char* name, GLfloat* vecArray, int numberOfVertices);
    void attribute2fv(const char* name, GLfloat* vecArray, int numberOfVertices);
};
