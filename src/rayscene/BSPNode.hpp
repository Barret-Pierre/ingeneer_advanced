#pragma once

#include <vector>
#include <memory>
#include "SceneObject.hpp"
#include "../raymath/AABB.hpp"

class BSPNode
{
public:
  AABB boundingBox;                    // Boîte englobante pour ce nœud
  std::vector<SceneObject *> objects;  // Objets dans ce nœud (feuille uniquement)
  std::unique_ptr<BSPNode> leftChild;  // Fils gauche (partition supérieure)
  std::unique_ptr<BSPNode> rightChild; // Fils droite (partition supérieure)

  BSPNode();

  const bool isLeaf() const;
};
