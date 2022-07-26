#pragma once

#include "Objects.h"
#include "System.hpp"
#include "glad.h"

#include <glm/gtc/type_ptr.hpp>
#include <unordered_map>
#include <vector>

class Camera
{
    glm::vec3 m_position;
    glm::vec3 mTarget;
    glm::vec3 mUp;
    glm::vec3 mStrafe;
    glm::vec2 mAngles = {3.14f, 0.0f};

    mutable glm::mat4 mMatrix;

    mutable bool mRecalculate;

    const glm::vec3 X_AXIS = glm::vec3(1.0f, 0.0f, 0.0f);
    const glm::vec3 Y_AXIS = glm::vec3(0.0f, 1.0f, 0.0f);
    const glm::vec3 Z_AXIS = glm::vec3(0.0f, 0.0f, 1.0f);

public:
    explicit Camera(const glm::vec3 &position, const glm::vec3 &target, const glm::vec3 &up) :
        m_position(position), mTarget(target), mUp(up), mStrafe(glm::normalize(glm::cross(mTarget, mUp))), mMatrix(1.0f), mRecalculate(true)
    {
    }

    void Move(const glm::vec3 &velocity);
    void Rotate(glm::vec2 angle);

    NODISCARD glm::mat4 CalculateMatrix() const;
    NODISCARD inline const glm::vec3 &GetPosition() const { return m_position; }
};

struct SDL_Window;

class Shader
{
    GLuint m_program_handle;

public:
    explicit Shader(const std::vector<std::string> &filePaths) : m_program_handle(CompileShader(filePaths)) {}

    void SetUniform(GLint location, const f32 value) { glUniform1f(location, value); }

    void SetUniform(GLint location, const glm::vec2 &value) { glUniform2fv(location, 1, glm::value_ptr(value)); }

    void SetUniform(GLint location, const glm::vec3 &value) { glUniform3fv(location, 1, glm::value_ptr(value)); }

    void SetUniform(GLint location, const glm::vec4 &value) { glUniform4fv(location, 1, glm::value_ptr(value)); }

    void SetUniform(GLint location, const glm::mat3 &value) { glUniformMatrix3fv(location, 1, false, glm::value_ptr(value)); }

    void SetUniform(GLint location, const glm::mat4 &value) { glUniformMatrix4fv(location, 1, false, glm::value_ptr(value)); }
    void Bind() const { glUseProgram(m_program_handle); }

private:
    GLuint CompileShader(const std::vector<std::string> &file_paths);

    std::vector<GLenum> DetermineShaderTypes(const std::vector<std::string> &file_paths);

    static char *readFile(const char *name);
    static void printShaderInfo(u32 shader);

    static void printProgramProgramInfo(const u32 shader);
};

class RenderSystem : public System
{
    // TODO: the Offset buffer should also be one large buffer with offsets
    struct RenderData {
        u32 vertex_buffer_offset;
        u32 vertex_buffer_size;
        u32 index_buffer_offset;
        u32 index_buffer_size;
    };
    std::unordered_map<Handle<objs::Mesh>, RenderData> m_meshes;
    Camera m_camera;

    GLuint m_vao = GL_NONE;
    struct MainBuffer {
        GLuint vertex_buffer = GL_NONE;
        GLuint vb_size = 0;
        GLuint index_buffer = GL_NONE;
        GLuint ib_size = 0;
    } m_main_buffer;
    SDL_Window *m_window = nullptr;

    static constexpr f32 SPEED = 5.0f;
    inline static const glm::mat4 PROJECTION = glm::infinitePerspective(90.0f, 16.0f / 9.0f, 0.01f);

    Shader m_phong{{"shaders/phong.vert", "shaders/phong.frag"}};

public:
    explicit RenderSystem(ResourceSystem *resource_system);
    void Step(f32 delta, const std::vector<Entity> &entities) override;
    void MeshLoaded(const Handle<objs::Mesh> &handle) override;
    SDL_Window *GetWindow() const { return m_window; }
};
