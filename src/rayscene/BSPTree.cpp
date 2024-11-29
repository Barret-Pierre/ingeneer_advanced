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

  // Condition de feuille en cas de profondeur maximale ou d'un petit nombre d'objets 10 / 20
  //? Why are we returning the node here?
  if (node->objects.size() <= 10 || depth >= 13)
  {
    return;
  }

  // Diviser les objets selon un plan
  int axis = depth % 3; // Axe de division : 0=x, 1=y, 2=z
  // double splitValue = (globalBox.getMin().getAxis(axis) + globalBox.getMax().getAxis(axis)) * 0.5;

  int axis = 0; // 0 = x, 1 = y, 2 = z
  Vector3 boxSize = node->boundingBox.getMax() - node->boundingBox.getMin();
  if (boxSize.y > boxSize.x && boxSize.y > boxSize.z)
    axis = 1; // y est dominant
  else if (boxSize.z > boxSize.x)
    axis = 2;

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
  // std::cout << "IntersectRecursive => ------------ Depth: " << depth << std::endl;
  // if (!node || !node->boundingBox.intersects(ray))
  //   return false;

  // Intersection intersection;
  // double closestDistance = -1;
  // Intersection closestInter;
  // bool hit = false;
  // Intersection tempClosest;

  // if (node->isLeaf())
  // {
  // std::cout << "IntersectRecursive => ------------ Leaf Depth: " << depth << ", Node id: " << node->id << std::endl;
  // for (SceneObject *object : node->objects)
  // {

  //   std::cout << "IntersectRecursive IS LEAF => Depth: " << depth << " Node id: " << node->id << " OBJECTS SIZE  : " << node->objects.size() << " / IS INTERSECT : " << object->intersects(ray, intersection, culling) << std::endl;
  //   if (object->intersects(ray, intersection, culling))
  //   {
  //     double distance = (closest.Position - ray.GetPosition()).lengthSquared();
  //     if (closestDistance < 0 || intersection.Distance < closestDistance)
  //     {
  //       closestDistance = intersection.Distance;
  //       closestInter = intersection;
  //     }
  //   }
  // }
  // closest = closestInter;
  // // std::cout << "IntersectRecursive => Depth: " << depth << " IS INTERSECT " << object->intersects(ray, tempClosest, culling) << std::endl;
  // return (closestDistance > -1);

  // for (SceneObject *obj : node->objects)
  // {
  //   if (obj->intersects(ray, tempClosest, culling))
  //   {
  //     double distance = (tempClosest.Position - ray.GetPosition()).lengthSquared();
  //     if (!hit || distance < closest.Distance)
  //     {
  //       hit = true;
  //       closest = tempClosest;
  //     }
  //   }
  // }

  // }
  // else
  // {
  // std::cout << "IntersectRecursive IS NOT LEAF => Depth: " << depth << " Node id: " << node->id << " OBJECTS SIZE  : " << node->objects.size() << std::endl;
  // Intersection leftClosest, rightClosest;
  // // std::cout << "IntersectRecursive => Depth: " << depth << " IS NOT LEAF" << std::endl;
  // bool hitLeft = intersectRecursive(ray, leftClosest, node->leftChild, culling, depth + 1);
  // bool hitRight = intersectRecursive(ray, rightClosest, node->rightChild, culling, depth + 1);
  // // std::cout << "IntersectRecursive => Depth: " << depth << " Left closets distance" << leftClosest.Distance << std::endl;
  // // std::cout << "IntersectRecursive => Depth: " << depth << " Right closets distance" << rightClosest.Distance << std::endl;

  // // Cas ou les noeud sont l'un derière l'autre
  // // if (hitLeft && hitRight)
  // // {
  // //   if ((node->leftChild->boundingBox.getMin() - ray.GetPosition()).lengthSquared() < (node->rightChild->boundingBox.getMin() - ray.GetPosition()).lengthSquared())
  // //   {
  // //     closest = leftClosest;
  // //   }
  // //   else
  // //   {
  // //     closest = rightClosest;
  // //   }
  // // }
  // // // si c'est le cas alors on prend le noeud avec la bounding box la plus proche
  // bool hitLeft = intersectRecursive(ray, closest, node->leftChild, culling, depth + 1);
  // bool hitRight = intersectRecursive(ray, closest, node->rightChild, culling, depth + 1);

  // hit = hitLeft || hitRight;

  // return hitLeft || hitRight;
  // }
  // return hit;
  if (!node || !node->boundingBox.intersects(ray))
    return false;

  if (node->isLeaf())
  {
    bool hit = false;
    for (SceneObject *object : node->objects)
    {
      Intersection tempClosest;
      if (object->intersects(ray, tempClosest, culling))
      {
        double distance = (tempClosest.Position - ray.GetPosition()).lengthSquared();
        if (!hit || distance < closest.Distance)
        {
          hit = true;
          closest = tempClosest;
        }
      }
    }
    return hit;
  }
  bool hitLeft = false, hitRight = false;
  Intersection leftClosest, rightClosest;

  if (node->leftChild && node->leftChild->boundingBox.intersects(ray) && node->leftChild->objects.size() > 0)
  {
    hitLeft = intersectRecursive(ray, leftClosest, node->leftChild, culling, depth + 1);
  }

  if (node->rightChild && node->rightChild->boundingBox.intersects(ray) && node->leftChild->objects.size() > 0)
  {
    hitRight = intersectRecursive(ray, rightClosest, node->rightChild, culling, depth + 1);
  }

  if (hitLeft && hitRight)
  {
    if ((leftClosest.Position - ray.GetPosition()).lengthSquared() <
        (rightClosest.Position - ray.GetPosition()).lengthSquared())
    {
      closest = leftClosest;
    }
    else
    {
      closest = rightClosest;
    }
    return true;
  }
  else if (hitLeft)
  {
    closest = leftClosest;
    return true;
  }
  else if (hitRight)
  {
    closest = rightClosest;
    return true;
  }

  return false;
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
  std::cout << "Node Depth: " << depth << ", Node id: " << node->id << ", Objects: " << node->objects.size()
            << ", BBox: [" << node->boundingBox.getMin() << " - " << node->boundingBox.getMax() << "]"
            << std::endl;

  printNode(node->leftChild, depth + 1);
  printNode(node->rightChild, depth + 1);
}
