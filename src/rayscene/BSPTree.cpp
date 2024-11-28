#include "BSPTree.hpp"

BSPTree::BSPTree() : root(new BSPNode()) {}

void BSPTree::build(const std::vector<SceneObject *> &objects, int depth)
{
  // Créer une boîte englobante pour tous les objets
  AABB globalBox = objects[0]->getBoundingBox();
  std::cout << "Build => Object 0: " << objects[0]->getBoundingBox().getMax() << objects[0]->getBoundingBox().getMin() << std::endl;
  for (size_t i = 1; i < objects.size(); ++i)
  {
    std::cout << "Build => Object " << objects[i] << ": " << objects[i]->getBoundingBox().getMax() << objects[i]->getBoundingBox().getMin() << std::endl;
    globalBox.subsume(objects[i]->getBoundingBox());
  }
  root->boundingBox = globalBox;
  root->objects = objects;

  buildRecursive(root, depth);
}

void BSPTree::buildRecursive(BSPNode *node, int depth)
{

  // std::cout << "BuildRecursive => ------------ Depth: " << depth << std::endl;
  if (node->objects.empty())
  {
    return;
  }
  // std::cout << "BuildRecursive =>  Depth: " << depth << " => boundingBox Min" << node->boundingBox.getMin() << " - boundingBox Max" << node->boundingBox.getMax() << std::endl;

  // Condition de feuille en cas de profondeur maximale ou d'un petit nombre d'objets
  //? Why are we returning the node here?
  if (node->objects.size() <= 2 || depth >= 10)
  {
    // std::cout << "BuildRecursive =>  Depth: " << depth << " => leaf condition" << std::endl;
    return;
  }

  // Diviser les objets selon un plan
  int axis = depth % 3; // Axe de division : 0=x, 1=y, 2=z
  // double splitValue = (globalBox.getMin().getAxis(axis) + globalBox.getMax().getAxis(axis)) * 0.5;

  // Diviser la global box en deux => AABB * 2
  auto [left, right] = node->boundingBox.split(axis);
  // std::cout << "BuildRecursive =>  Depth: " << depth << " => left Min" << left.getMin() << " - left Max" << left.getMax() << std::endl;
  // std::cout << "BuildRecursive =>  Depth: " << depth << " => right Min" << right.getMin() << " - right Max" << right.getMax() << std::endl;

  // std::cout << "BuildRecursive => Depth: " << depth << " => object put in left" << std::endl;
  std::vector<SceneObject *> leftObjects;
  std::vector<SceneObject *> rightObjects;

  for (SceneObject *object : node->objects)
  {
    // std::cout << "BuildRecursive => Object: " << object << std::endl;

    AABB boundingBox = object->getBoundingBox();
    if (left.overlaps(boundingBox))
    {
      // std::cout << "BuildRecursive => Depth: " << depth << " => object put in left" << std::endl;
      leftObjects.push_back(object);
    }
    if (right.overlaps(boundingBox))
    {
      // std::cout << "BuildRecursive => Depth: " << depth << " => object put in left" << std::endl;
      rightObjects.push_back(object);
    }
  }

  // for (int i = 0; i < leftObjects.size(); ++i)
  // {
  //   std::cout << "BuildRecursive => LeftObjects " << leftObjects[i] << ": " << leftObjects[i]->getBoundingBox().getMax() << leftObjects[i]->getBoundingBox().getMin() << std::endl;
  // }
  // for (int i = 0; i < rightObjects.size(); ++i)
  // {
  //   std::cout << "BuildRecursive => RightObjects " << rightObjects[i] << ": " << rightObjects[i]->getBoundingBox().getMax() << rightObjects[i]->getBoundingBox().getMin() << std::endl;
  // }

  // Construire récursivement les sous-nœuds
  // std::cout << "BuildRecursive => Depth: " << depth << " => new nodes construct" << std::endl;
  node->leftChild = new BSPNode();
  node->leftChild->boundingBox = left;
  node->leftChild->objects = leftObjects;
  buildRecursive(node->leftChild, depth + 1);
  node->rightChild = new BSPNode();
  node->rightChild->boundingBox = right;
  node->rightChild->objects = rightObjects;
  buildRecursive(node->rightChild, depth + 1);
}

bool BSPTree::intersect(Ray &ray, Intersection &closest, CullingType culling)
{
  return intersectRecursive(ray, closest, root, culling, 0);
}

bool BSPTree::intersectRecursive(Ray &ray, Intersection &closest, BSPNode *node, CullingType culling, int depth)
{
  std::cout << "IntersectRecursive => ------------ Depth: " << depth << std::endl;
  if (!node || node->boundingBox.intersects(ray))
    return false;

  bool hit = false;
  Intersection tempClosest;

  if (node->isLeaf())
  {
    for (SceneObject *obj : node->objects)
    {
      if (obj->intersects(ray, tempClosest, culling))
      {
        double distance = (tempClosest.Position - ray.GetPosition()).lengthSquared();
        if (!hit || distance < closest.Distance)
        {
          hit = true;
          closest = tempClosest;
        }
      }
    }
  }
  else
  {
    Intersection leftClosest, rightClosest;
    bool hitLeft = intersectRecursive(ray, closest, node->leftChild, culling, depth + 1);
    bool hitRight = intersectRecursive(ray, closest, node->rightChild, culling, depth + 1);

    hit = hitLeft || hitRight;
  }

  return hit;
}

void BSPTree::printTree() const
{
  printNode(root, 0);
}

void BSPTree::printNode(BSPNode *node, int depth) const
{
  if (!node)
    return;

  // Indentation pour représenter la profondeur
  for (int i = 0; i < depth; ++i)
    std::cout << "  ";
  std::cout << "Node Depth: " << depth << ", Objects: " << node->objects.size()
            << ", BBox: [" << node->boundingBox.getMin() << " - " << node->boundingBox.getMax() << "]"
            << std::endl;

  printNode(node->leftChild, depth + 1);
  printNode(node->rightChild, depth + 1);
}
