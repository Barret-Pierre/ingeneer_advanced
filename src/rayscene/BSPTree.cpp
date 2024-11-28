#include "BSPTree.hpp"

BSPTree::BSPTree() : root(new BSPNode()) {}

void BSPTree::build(const std::vector<SceneObject *> &objects, int depth)
{
  *root = buildRecursive(objects, depth);
}

BSPNode BSPTree::buildRecursive(const std::vector<SceneObject *> &objects, int depth)
{
  std::cout << "buildRecursive" << std::endl;
  BSPNode node = BSPNode();

  if (objects.empty())
  {
    return node;
  }

  // Créer une boîte englobante pour tous les objets
  AABB globalBox = objects[0]->getBoundingBox();
  for (size_t i = 1; i < objects.size(); ++i)
  {
    globalBox.subsume(objects[i]->getBoundingBox());
  }
  node.boundingBox = globalBox;

  // Condition de feuille : peu d'objets ou profondeur max atteinte
  //? Why are we returning the node here?
  if (objects.size() <= 1 || depth >= 16)
  {
    std::cout << "leaf condition" << std::endl;
    node.objects = objects;
    return node;
  }

  // Diviser les objets selon un plan
  int axis = depth % 3; // Axe de division : 0=x, 1=y, 2=z
  double splitValue = (globalBox.getMin().getAxis(axis) + globalBox.getMax().getAxis(axis)) * 0.5;

  std::vector<SceneObject *> leftObjects;
  std::vector<SceneObject *> rightObjects;

  for (SceneObject *obj : objects)
  {
    AABB objBox = obj->getBoundingBox();
    if (objBox.getMax().getAxis(axis) <= splitValue)
      leftObjects.push_back(obj);
    else if (objBox.getMin().getAxis(axis) >= splitValue)
      rightObjects.push_back(obj);
    else
    {
      // Si un objet chevauche, ajoutez-le aux deux côtés
      leftObjects.push_back(obj);
      rightObjects.push_back(obj);
    }
  }

  // Construire récursivement les sous-nœuds
  std::cout << "new nodes construct" << std::endl;
  BSPNode leftChild = BSPNode();
  BSPNode rightChild = BSPNode();
  node.leftChild = std::make_unique<BSPNode>(buildRecursive(leftObjects, depth + 1));
  node.rightChild = std::make_unique<BSPNode>(buildRecursive(rightObjects, depth + 1));

  return node;
}

bool BSPTree::intersect(Ray &ray, Intersection &closest, CullingType culling)
{
  return intersectRecursive(ray, closest, root, culling);
}

bool BSPTree::intersectRecursive(Ray &ray, Intersection &closest, const std::unique_ptr<BSPNode> &node, CullingType culling)
{
  // std::cout << "intersectRecursive" << std::endl;
  if (!node || !node->boundingBox.intersects(ray))
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
    bool hitLeft = intersectRecursive(ray, closest, node->leftChild, culling);
    bool hitRight = intersectRecursive(ray, closest, node->rightChild, culling);

    hit = hitLeft || hitRight;
  }

  return hit;
}
