#pragma once

#include <vector>
#include <set>
#include <vector>
#include <memory>
#include "SceneObject.hpp"
#include "../raymath/AABB.hpp"

#include <atomic>

class UniqueIDGenerator
{
public:
  static int generateID()
  {
    static std::atomic<int> counter{0};
    return counter++;
  }
};

class BSPNode
{
public:
  AABB boundingBox;                   // Boîte englobante pour ce nœud
  std::vector<SceneObject *> objects; // Objets dans ce nœud (feuille uniquement)
  BSPNode *leftChild;                 // Fils gauche (partition supérieure)
  BSPNode *rightChild;
  int id = UniqueIDGenerator::generateID();

  BSPNode();
  ~BSPNode();

  const bool isLeaf() const;
};
