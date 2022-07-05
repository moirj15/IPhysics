#include "ResourceSystem.hpp"

#include "tiny_obj_loader.h"

Handle<objs::Mesh> ResourceSystem::LoadMesh(const std::string &path)
{
    objs::Mesh mesh;
    // Load our mesh if we got a good path
    if (std::filesystem::exists(path)) {
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
    } else {
        // TODO: pop-up or something
    }
}
