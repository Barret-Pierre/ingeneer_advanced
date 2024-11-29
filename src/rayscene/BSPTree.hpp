#pragma once

#include <vector>
#include <fstream>

#include "SceneObject.hpp"
#include "Intersection.hpp"
#include "../raymath/Ray.hpp"
#include "BSPNode.hpp"

class BSPTree
{
public:
  BSPNode *root;

  BSPTree();
  ~BSPTree();

  void build(const std::vector<SceneObject *> &objects, int depth = 0);
  std::set<SceneObject *> intersect(Ray &ray, Intersection &closest, CullingType culling);
  void buildRecursive(BSPNode *node, int depth = 0);
  std::set<SceneObject *> intersectRecursive(Ray &ray, Intersection &closest, BSPNode *node, CullingType culling, int depth = 0);
  void printNode(BSPNode *node, int depth) const;
  void printTree() const;
  void exportToDot(const std::string &filename) const;
  void exportNodeToDot(BSPNode *node, std::ofstream &dotFile) const;
};
