
#pragma once

#include "../common.h"
#include "ShaderData.h"
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

namespace Renderer
{
Window *InitAPI(const s32 width, const s32 height, const char *windowName, bool enableDebug);

enum class CommandType : u32
{
  DrawSolid,
  DrawSolidFlatShade,
  DrawSolidPhong,
  DrawLine,
  DrawPoints,
  DrawTextured,
  UpdateMesh,
  ClearDepthBuffer,
};

struct DrawCommand
{
  CommandType mType;
  u32 mMeshHandle = 0;
  u32 mTextureHandle = 0;
  Mesh *mMesh = nullptr;
  Texture2D *mTexture2D = nullptr;
  std::vector<ShaderData> mShaderData;

  DrawCommand() = default;
  /**
   * \brief: Construct a DrawCommand object.
   * \param type: The CommandType, the only required member.
   * \param meshHandle: A handle to some mesh.
   * \param textureHandle: A handle to some texture.
   * \param mesh: A pointer to a mesh.
   * \param color: A color.
   * \param texture2D: A pointer to a Texture2D object.
   */
  explicit DrawCommand(
      const CommandType type, const u32 meshHandle = 0, const u32 textureHandle = 0,
      Mesh *mesh = nullptr, Texture2D *texture2D = nullptr) :
      mType(type),
      mMeshHandle(meshHandle), mTextureHandle(textureHandle), mMesh(mesh)
  {
  }
  explicit DrawCommand(
      const CommandType type, const u32 meshHandle, std::vector<ShaderData> &shaderData) :
      mType(type),
      mMeshHandle(meshHandle), mShaderData(shaderData)
  {
  }
};

class RendererBackend
{
  Window *mWindow;
  std::vector<DrawCommand> mCommandQueue;
  ShaderLibrary *mShaderLibrary;
  TextureLibrary *mTextureLibrary;
  MeshLibrary *mMeshLibrary;

public:
  /**
   * \brief: Construct a RendererBackend object.
   * \param window: Pointer to a window which the renderer will render to.
   */
  explicit RendererBackend(Window *window);

  /**
   * \brief: Submit a RenderCommand to the command queue.
   * \param command: The RenderCommand object that will be submitted.
   */
  inline void SubmitCommand(DrawCommand &command)
  {
    mCommandQueue.emplace_back(command);
  }

  /**
   * \brief: Clear the command queue.
   */
  inline void ClearCommandQueue()
  {
    // mCommandQueue.clear();
  }

  /**
   * \brief: Submit a mesh to be stored in the renderer's MeshLibrary.
   * \param mesh: The mesh that will be stored.
   * \return: A handle for accessing the mesh.
   */
  u32 SubmitMesh(Mesh *mesh);
  /**
   * \brief: Submit a texture to be stored in the renderer's TextureLibrary.
   * \param texture: The texture that will be stored.
   * \return: A handle for accessing the texture.
   */
  u32 SubmitTexture(Texture2D *texture);
  void Draw();
  /**
   * \brief: Updates the screen with the results of a Draw() call.
   */
  void UpdateScreen();

  void SetClearColor(const glm::vec4 &color);

  void ClearScreen();
  void RemoveMesh(u32 handle);

private:
  /**
   * \brief: Get the shader that will be used with the given CommandType, and optionally bind it.
   * \param type: The CommandType.
   * \param bind: If true then the shader will be bound before it's returned, otherwise it will just
   * be returned.
   * \return: A pointer to the shader.
   */
  Shader *GetShader(const CommandType type, bool bind = false);

  // TODO: refractor this.
  /**
   * \brief: Retrieves the buffers with the given handle and binds them before returning them.
   * \param handle: A handle to the buffers.
   * \return: The buffers.
   */
  IndexBuffer GetBuffersAndBind(u32 handle);
};

} // namespace Renderer
