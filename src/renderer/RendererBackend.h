
#pragma once

#include "../common.h"
#include "../utils/VoxelMeshManager.h"
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
};

struct DrawCommand
{
  CommandType mType;
  u32 mMeshHandle = 0;
  u32 mTextureHandle = 0;
  //   Mesh *mMesh = nullptr;
  //   Texture2D *mTexture2D = nullptr;
  std::vector<ShaderData> mShaderData;
  bool mPersist = false;

  DrawCommand() = default;
  /**
   * \brief: Construct a DrawCommand object.
   * \param type: The CommandType, the only required member.
   * \param meshHandle: A handle to some mesh.
   */
  DrawCommand(
      const CommandType type, const u32 meshHandle, std::vector<ShaderData> &shaderData,
      bool persist = false) :
      mType(type),
      mMeshHandle(meshHandle), mShaderData(shaderData), mPersist(persist)
  {
  }
};

class RendererBackend
{
  Window *mWindow;
  std::vector<DrawCommand> mCommandQueue;
  std::vector<DrawCommand> mPersistentDrawCommands;
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
    if (command.mPersist)
    {
      mPersistentDrawCommands.emplace_back(command);
    }
    else
    {
      mCommandQueue.emplace_back(command);
    }
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
  void UpdateMesh(const u32 handle, const std::vector<u32> &verts, Mesh *mesh);

private:
  void DrawFromCommand(const DrawCommand &command);

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
