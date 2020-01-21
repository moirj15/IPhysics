
#pragma once

#include "../common.h"
#include "indexBufferObject.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <string>
#include <vector>

struct Mesh;
struct Window;
struct Texture2D;
class Camera;
class Shader;
class ShaderLibrary;
class TextureLibrary;
class MeshLibrary;

enum class CommandType : u32
{
  DrawSolid,
  DrawLine,
  DrawPoints,
  DrawTextured,
  UpdateMesh,
  ClearDepthBuffer,
};

struct Command
{
  CommandType mType;
  u32 mMeshHandle = 0;
  u32 mTextureHandle = 0;
  Mesh *mMesh = nullptr;
  glm::vec3 mColor = {0.0f, 0.0f, 0.0f};
  Texture2D *mTexture2D = nullptr;

  /**
   * \brief: Construct a Command object.
   * @param type: The CommandType, the only required member.
   * @param meshHandle: A handle to some mesh.
   * @param textureHandle: A handle to some texture.
   * @param mesh: A pointer to a mesh.
   * @param color: A color.
   * @param texture2D: A pointer to a Texture2D object.
   */
  Command(
      const CommandType type, const u32 meshHandle = 0, const u32 textureHandle = 0,
      Mesh *mesh = nullptr, const glm::vec3 &color = {0.0f, 0.0f, 0.0f},
      Texture2D *texture2D = nullptr) :
      mType(type),
      mMeshHandle(meshHandle), mTextureHandle(textureHandle), mMesh(mesh), mColor(color)
  {
  }
};

class Renderer
{
  Window *mWindow;
  std::vector<Command> mCommandQueue;
  ShaderLibrary *mShaderLibrary;
  TextureLibrary *mTextureLibrary;
  MeshLibrary *mMeshLibrary;

public:
  /**
   * \brief: Construct a Renderer object.
   * @param window: Pointer to a window which the renderer will render to.
   */
  explicit Renderer(Window *window);

  /**
   * \brief: Submit a RenderCommand to the command queue.
   * @param command: The RenderCommand object that will be submitted.
   */
  inline void SubmitCommand(Command &command)
  {
    mCommandQueue.emplace_back(command);
  }

  /**
   * \brief: Clear the command queue.
   */
  inline void ClearCommandQueue()
  {
    mCommandQueue.clear();
  }

  /**
   * \brief: Submit a mesh to be stored in the renderer's MeshLibrary.
   * @param mesh: The mesh that will be stored.
   * @return: A handle for accessing the mesh.
   */
  u32 SubmitMesh(Mesh *mesh);
  /**
   * \brief: Submit a texture to be stored in the renderer's TextureLibrary.
   * @param texture: The texture that will be stored.
   * @return: A handle for accessing the texture.
   */
  u32 SubmitTexture(Texture2D *texture);
  void Draw(Camera *camera, const glm::mat4 &projection);
  void UpdateScreen();

private:
  Shader *GetShader(const CommandType type, bool bind = false);
  IndexBuffer GetBuffersAndBind(const u32 handle);
};
