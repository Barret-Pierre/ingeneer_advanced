#include "BSPNode.hpp"

BSPNode::BSPNode() : leftChild(nullptr), rightChild(nullptr)
{
}

BSPNode::~BSPNode()
{
}

// Est une feuille si aucun des deux noeud enfant ne contient d'objet
const bool BSPNode::isLeaf() const
{
  return leftChild == nullptr && rightChild == nullptr && !objects.empty();
}
