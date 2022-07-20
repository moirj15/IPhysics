#pragma once

#include "Handle.hpp"
#include <glm/vec3.hpp>

namespace objs { struct Mesh; }
struct DeformationBuffer;

struct PhysicalProperties {
    glm::vec3 pos;
    glm::vec3 initial_velocity;
    f32 mass;
};

struct Entity {
    Handle<objs::Mesh> mesh;
    Handle<DeformationBuffer> deformation_buffer;
    PhysicalProperties physical_properties;
};