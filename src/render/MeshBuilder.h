#ifndef PLANETARY_OBSERVATORY_RENDER_MESHBUILDER_H
#define PLANETARY_OBSERVATORY_RENDER_MESHBUILDER_H

#include "common/EOGL.h"

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include <vector>

struct MeshData {
  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec2> texCoords;
  std::vector<unsigned int> indices;
};

MeshData buildSphere(float radius, int slices, int stacks);

#endif // PLANETARY_OBSERVATORY_RENDER_MESHBUILDER_H
