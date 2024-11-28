#pragma once

#include <vector>

#include "SceneObject.hpp"
#include "Intersection.hpp"
#include "../raymath/Ray.hpp"
#include "BSPNode.hpp"

class BSPTree
{
public:
  std::unique_ptr<BSPNode> root;

  BSPTree();

  void build(const std::vector<SceneObject *> &objects, int depth = 0);
  bool intersect(Ray &ray, Intersection &closest, CullingType culling);
  BSPNode buildRecursive(const std::vector<SceneObject *> &objects, int depth = 0);
  bool intersectRecursive(Ray &ray, Intersection &closest, const std::unique_ptr<BSPNode> &node, CullingType culling);
};
