#include "ResourceSystem.hpp"

#include "System.hpp"
#include "tiny_obj_loader.h"

MeshHandle ResourceSystem::LoadMesh(const std::string &path)
{
    objs::Mesh mesh;
    auto handle = MeshHandle::Invalid();
    // Load our mesh if we got a good path
    if (m_mesh_cache.contains(path)) {
        handle = m_mesh_cache[path];
    } else if (std::filesystem::exists(path)) {
        tinyobj::ObjReader reader;
        reader.ParseFromFile(path);
        auto attrib = reader.GetAttrib();
        // assuming one shape for now
        auto shape = reader.GetShapes()[0];
        // TODO: calculate a better index buffer
        for (auto indices : shape.mesh.indices) {
            mesh.vertices.push_back(attrib.vertices[indices.vertex_index * 3]);
            mesh.vertices.push_back(attrib.vertices[(indices.vertex_index * 3) + 1]);
            mesh.vertices.push_back(attrib.vertices[(indices.vertex_index * 3) + 2]);

            u32 normalIndex = indices.normal_index != -1 ? indices.normal_index : indices.vertex_index;
            mesh.normals.push_back(attrib.normals[normalIndex * 3]);
            mesh.normals.push_back(attrib.normals[(normalIndex * 3) + 1]);
            mesh.normals.push_back(attrib.normals[(normalIndex * 3) + 2]);

            mesh.indices.push_back(mesh.indices.size());
        }
        m_next_handle++;
        handle = MeshHandle(m_next_handle);
        m_mesh_cache.emplace(path, handle);
        m_meshes.emplace(handle, mesh);
    }
    if (handle) {
        for (auto *listener : m_mesh_listeners) {
            listener->MeshLoaded(handle);
        }
    }
    return handle;
}
