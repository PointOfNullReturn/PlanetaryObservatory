#include "render/MeshBuilder.h"

#include <cmath>
#include <glm/geometric.hpp>

MeshData buildSphere(float radius, int slices, int stacks) {
  MeshData mesh;
  const int vertexCount = (stacks + 1) * (slices + 1);
  mesh.positions.reserve(static_cast<std::size_t>(vertexCount));
  mesh.normals.reserve(static_cast<std::size_t>(vertexCount));
  mesh.texCoords.reserve(static_cast<std::size_t>(vertexCount));

  for (int stack = 0; stack <= stacks; ++stack) {
    const float v = static_cast<float>(stack) / static_cast<float>(stacks);
    const float phi = v * static_cast<float>(M_PI);
    const float sinPhi = std::sin(phi);
    const float cosPhi = std::cos(phi);

    for (int slice = 0; slice <= slices; ++slice) {
      const float u = static_cast<float>(slice) / static_cast<float>(slices);
      const float theta = u * 2.0f * static_cast<float>(M_PI);
      const float sinTheta = std::sin(theta);
      const float cosTheta = std::cos(theta);

      glm::vec3 normal{sinPhi * cosTheta, cosPhi, sinPhi * sinTheta};
      glm::vec3 position = radius * normal;

      mesh.positions.push_back(position);
      mesh.normals.push_back(glm::normalize(normal));
      mesh.texCoords.emplace_back(u, 1.0f - v);
    }
  }

  for (int stack = 0; stack < stacks; ++stack) {
    for (int slice = 0; slice < slices; ++slice) {
      const int first = stack * (slices + 1) + slice;
      const int second = first + slices + 1;

      mesh.indices.push_back(static_cast<unsigned int>(first));
      mesh.indices.push_back(static_cast<unsigned int>(second));
      mesh.indices.push_back(static_cast<unsigned int>(first + 1));

      mesh.indices.push_back(static_cast<unsigned int>(second));
      mesh.indices.push_back(static_cast<unsigned int>(second + 1));
      mesh.indices.push_back(static_cast<unsigned int>(first + 1));
    }
  }

  return mesh;
}
