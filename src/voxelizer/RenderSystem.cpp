#include "RenderSystem.hpp"

#include "Entity.hpp"
#include "ResourceSystem.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <SDL2/SDL.h>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/vec3.hpp>

void Camera::Move(const glm::vec3 &velocity)
{
    mRecalculate = true;
    m_position += (mStrafe * velocity.x) + (mUp * velocity.y) + (mTarget * velocity.z);
}

void Camera::Rotate(glm::vec2 angle)
{
    mRecalculate = true;
    mAngles += glm::radians(angle * 0.005f);
    if (mAngles.y > glm::radians(89.0f)) {
        mAngles.y = glm::radians(89.0f);
    }
    if (mAngles.y < glm::radians(-89.0f)) {
        mAngles.y = glm::radians(-89.0f);
    }

    mTarget = glm::normalize(glm::vec3(cos(mAngles.y) * sin(mAngles.x), sin(mAngles.y), cos(mAngles.y) * cos(mAngles.x)));
    mStrafe = glm::cross(mTarget, mUp);
}

glm::mat4 Camera::CalculateMatrix() const
{
    if (mRecalculate) {
        mRecalculate = false;
        mMatrix = glm::lookAt(m_position, m_position + mTarget, mUp);
        return mMatrix;
    } else {
        return mMatrix;
    }
}

struct ConastantsLayout {
    glm::mat4 camera;
    glm::mat4 mvp;
    glm::mat4 normal_mat;
    glm::vec4 light_pos;
};

namespace PhongUniform
{
constexpr GLuint Camera = 0;
constexpr GLuint Mvp = 1;
constexpr GLuint NormalMat = 2;
constexpr GLuint LightPosition = 3;
constexpr GLuint LightColor = 4;
constexpr GLuint AmbientLight = 5;
constexpr GLuint AmbientColor = 6;
constexpr GLuint DiffuseColor = 7;
constexpr GLuint SpecularColor = 8;
constexpr GLuint Coeff = 9;
}; // namespace PhongUniform

struct MeshBuffer {
    glm::vec3 vertex;
    glm::vec3 normal;

    static constexpr GLuint Size() { return sizeof(MeshBuffer); }
    static constexpr const void *VertexOffset() { return reinterpret_cast<const void *>(offsetof(MeshBuffer, vertex)); }
    static constexpr const void *NormalOffset() { return reinterpret_cast<const void *>(offsetof(MeshBuffer, normal)); }
};

namespace MeshBufferLayout
{
constexpr GLuint Vertices = 0;
constexpr GLuint Normals = 1;
} // namespace MeshBufferLayout

GLuint Shader::CompileShader(const std::vector<std::string> &file_paths)
{
    GLuint program_handle = glCreateProgram();
    s32 flag;
    std::vector<GLenum> shaderTypes = DetermineShaderTypes(file_paths);

    for (Size i = 0; i < file_paths.size(); i++) {
        const auto filename = file_paths[i].c_str();
        const auto shaderType = shaderTypes[i];
        char *shaderSrc = nullptr;

        const u32 shaderRef = glCreateShader(shaderType);

        shaderSrc = readFile(filename);

        glShaderSource(shaderRef, 1, static_cast<GLchar **>(&shaderSrc), nullptr);

        delete[](shaderSrc);

        glCompileShader(shaderRef);
        glGetShaderiv(shaderRef, GL_COMPILE_STATUS, &flag);
        printShaderInfo(shaderRef);

        if (!flag) {
            fprintf(stderr, "failed to compile frag shader\n");
            // exit(EXIT_FAILURE);
        }

        glAttachShader(m_program_handle, shaderRef);
    }
    glLinkProgram(m_program_handle);
    glGetProgramiv(m_program_handle, GL_LINK_STATUS, &flag);

    printProgramProgramInfo(m_program_handle);

    if (!flag) {
        fprintf(stderr, "failed to link shaders");
        // exit(EXIT_FAILURE);
    }
    return program_handle;
}

std::vector<GLenum> Shader::DetermineShaderTypes(const std::vector<std::string> &file_paths)
{
    std::vector<GLenum> ret;
    for (const auto &src : file_paths) {
        std::string type = src.substr(src.size() - 4);
        if (type == "vert") {
            ret.push_back(GL_VERTEX_SHADER);
        } else if (type == "frag") {
            ret.push_back(GL_FRAGMENT_SHADER);
        }
    }
    return ret;
}

char *Shader::readFile(const char *name)
{
    FILE *fp = nullptr;
    fp = fopen(name, "r");
    char *src = nullptr;
    uint64_t len = 0;

    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    rewind(fp);

    if (len == 0) {
        fprintf(stderr, "failed to get file size");
        // exit(EXIT_FAILURE);
    }

    src = (char *)calloc(len + 1, sizeof(char));
    len = fread(src, sizeof(char), len, fp);

    fclose(fp);

    return src;
}

void Shader::printShaderInfo(u32 shader)
{
    s32 length = 0;
    GLsizei nchars = 0;
    char *log = nullptr;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

    if (length > 0) {
        log = new char[length];

        if (log != nullptr) {
            glGetShaderInfoLog(shader, length, &nchars, log);

            if (log[0] != '\0') {
                printf("shader log: '%s'\n", log);
            }

            delete[](log);
        }
    }
}

void Shader::printProgramProgramInfo(const u32 shader)
{
    GLint length = 0;
    GLsizei nchars = 0;
    char *log = nullptr;

    glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &length);

    if (length > 0) {
        log = new char[length];

        if (log) {
            glGetProgramInfoLog(shader, length, &nchars, log);

            if (log[0] != '\0') {
                printf("program log: '%s'\n", log);
            }

            delete[](log);
        }
    }
}

RenderSystem::RenderSystem(ResourceSystem *resource_system) : System(resource_system), m_camera({0, 0, 0}, {0, 0, -1}, {0, 1, 0})
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    constexpr u32 width = 1920;
    constexpr u32 height = 1080;

    m_window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);

    assert(m_window != nullptr);

    // Don't resize window so it doesn't mess with tiling window managers
    SDL_SetWindowResizable(m_window, SDL_FALSE);

    auto *context = SDL_GL_CreateContext(m_window);
    assert(context);
    assert(gladLoadGL());

#ifdef _DEBUG
    // TODO: add debug flag
//    glEnable(GL_DEBUG_OUTPUT);
//    glDebugMessageCallback(DBCallBack, nullptr);
//    printf("%s", glGetString(GL_VERSION));
#endif
    glEnable(GL_PROGRAM_POINT_SIZE);

    glGenVertexArrays(1, &m_vao);
}

void RenderSystem::Step(f32 delta, const std::vector<Entity> &entities)
{
    for (const auto &event : m_event_queue) {
        if (event == Event::MoveForward) {
            m_camera.Move(delta * SPEED * glm::vec3(0, 0, -1));
        } else if (event == Event::MoveBackwards) {
            m_camera.Move(delta * SPEED * glm::vec3(0, 0, 1));
        } else if (event == Event::MoveLeft) {
            m_camera.Move(delta * SPEED * glm::vec3(-1, 0, 0));
        } else if (event == Event::MoveRight) {
            m_camera.Move(delta * SPEED * glm::vec3(1, 0, 0));
        } else if (event == Event::MoveUp) {
            m_camera.Move(delta * SPEED * glm::vec3(0, 1, 0));
        } else if (event == Event::MoveDown) {
            m_camera.Move(delta * SPEED * glm::vec3(0, -1, 0));
        } else if (event == Event::MoveBoost) {
        }
    }

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_main_buffer.vertex_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_main_buffer.index_buffer);

    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(MeshBufferLayout::Vertices, 3, GL_FLOAT, false, MeshBuffer::Size(), MeshBuffer::VertexOffset());
    glVertexAttribPointer(MeshBufferLayout::Normals, 3, GL_FLOAT, false, MeshBuffer::Size(), MeshBuffer::NormalOffset());
    // TODO: bind deformation buffer

    m_phong.Bind();
constexpr GLuint mvp = 1;
constexpr GLuint normalMat = 2;

    const auto camera = m_camera.CalculateMatrix();
    m_phong.SetUniform(PhongUniform::Camera, camera);
    m_phong.SetUniform(PhongUniform::LightPosition, glm::vec4(0.0f, 10.0f, 0.0f, 1.0f));
    m_phong.SetUniform(PhongUniform::LightColor, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    m_phong.SetUniform(PhongUniform::AmbientLight, glm::vec4(0.5f, 0.3f, 0.2f, 0.0f));
    m_phong.SetUniform(PhongUniform::AmbientColor, glm::vec4(0.5f, 0.3f, 0.2f, 0.0f));
    m_phong.SetUniform(PhongUniform::DiffuseColor, glm::vec4(0.5f, 0.3f, 0.2f, 0.0f));
    m_phong.SetUniform(PhongUniform::SpecularColor, glm::vec4(0.5f, 0.3f, 0.2f, 0.0f));
    m_phong.SetUniform(PhongUniform::Coeff, glm::vec4(1.0f, 1.0f, 1.0f, 10.0f));


    for (const auto &entity : entities) {
        // TODO: scale and rotate
        const auto model = glm::translate(glm::mat4(1.0), entity.physical_properties.pos);
        const auto mvp = PROJECTION * camera * model;
        const auto normal_mat = glm::inverseTranspose(camera * model);   

        m_phong.SetUniform(PhongUniform::Mvp, mvp);
        m_phong.SetUniform(PhongUniform::NormalMat, normal_mat);

        const auto render_data = m_meshes[entity.mesh];

        glDrawElementsBaseVertex(GL_TRIANGLES, render_data.index_buffer_size, GL_UNSIGNED_INT, reinterpret_cast<const void *>(render_data.index_buffer_offset));
    }

    SDL_GL_SwapWindow(m_window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderSystem::MeshLoaded(const Handle<objs::Mesh> &handle)
{
    if (!m_meshes.contains(handle)) {
        const auto mesh = m_resources_system->GetMesh(handle);

        const auto BufferPushBack = [](GLuint &buffer, GLuint &buffer_size, const void *new_data, GLuint new_data_size) {
            GLuint new_buffer = GL_NONE;
            GLuint staging_buffer = GL_NONE;

            GLuint buffers[2] = {};
            glCreateBuffers(2, buffers);
            new_buffer = buffers[0];
            staging_buffer = buffers[1];
            // NOTE: might need to use NamedBufferData instead incase this is _really_ immutable
            glNamedBufferStorage(new_buffer, static_cast<GLsizeiptr>(buffer_size + new_data_size), nullptr, GL_NONE);
            glCopyNamedBufferSubData(buffer, new_buffer, 0, 0, buffer_size);

            glNamedBufferStorage(staging_buffer, static_cast<GLsizeiptr>(new_data_size), new_data, GL_NONE);
            glCopyNamedBufferSubData(staging_buffer, new_buffer, 0, buffer_size, static_cast<GLsizeiptr>(new_data_size));
            glDeleteBuffers(1, &staging_buffer);
            glDeleteBuffers(1, &buffer);
            buffer = new_buffer;
            buffer_size = buffer_size + new_data_size;
        };
        const auto staging_data = mesh.GetInterleved();
        RenderData data = {
            .vertex_buffer_offset = m_main_buffer.vb_size,
            .vertex_buffer_size = static_cast<u32>(staging_data.size() * sizeof(f32)),
            .index_buffer_offset = m_main_buffer.ib_size,
            .index_buffer_size = mesh.IndicesSizeInBytes(),
        };
        BufferPushBack(m_main_buffer.vertex_buffer, m_main_buffer.vb_size, static_cast<const void *>(staging_data.data()), data.vertex_buffer_size);
        BufferPushBack(m_main_buffer.index_buffer, m_main_buffer.ib_size, static_cast<const void *>(mesh.indices.data()), data.index_buffer_size);
        m_meshes.emplace(handle, data);
    }
}
