#include "BSPTree.hpp"

BSPTree::BSPTree() : root(new BSPNode()) {}

BSPTree::~BSPTree()
{
}

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

  // Condition de feuille en cas de profondeur maximale ou d'un petit nombre d'objets 10 / 13
  //? Why are we returning the node here?
  if (node->objects.size() <= 1 || depth >= 4)
  {
    return;
  }

  // Diviser les objets selon un plan
  // int axis = depth % 3; // Axe de division : 0=x, 1=y, 2=z
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

std::set<SceneObject *> BSPTree::intersect(Ray &ray, Intersection &closest, CullingType culling)
{
  return intersectRecursive(ray, closest, root, culling, 0);
}

std::set<SceneObject *> BSPTree::intersectRecursive(Ray &ray, Intersection &closest, BSPNode *node, CullingType culling, int depth)
{
  std::cout << "Node: " << node->id << std::endl;
  std::set<SceneObject *> intersectedObjects;

  if (!node || !node->boundingBox.intersects(ray))
    std::cout << "NO INTERSECT: " << node->id << std::endl;
  return intersectedObjects;

  if (node->isLeaf())
  {
    std::cout << "LEAF: " << node->id << std::endl;
    for (SceneObject *object : node->objects)
    {
      intersectedObjects.insert(object);
    }
    return intersectedObjects;
  }

  std::cout << "NO LEAF: " << node->id << std::endl;
  bool hitLeft = false, hitRight = false;
  Intersection leftClosest, rightClosest;

  if (node->leftChild && node->leftChild->boundingBox.intersects(ray) && node->leftChild->objects.size() > 0)
  {
    std::set<SceneObject *> intersectedLeftObjects = intersectRecursive(ray, leftClosest, node->leftChild, culling, depth + 1);
    intersectedObjects.insert(intersectedLeftObjects.begin(), intersectedLeftObjects.end());
  }

  if (node->rightChild && node->rightChild->boundingBox.intersects(ray) && node->rightChild->objects.size() > 0)
  {
    std::set<SceneObject *> intersectedRightObjects = intersectRecursive(ray, rightClosest, node->rightChild, culling, depth + 1);
    intersectedObjects.insert(intersectedRightObjects.begin(), intersectedRightObjects.end());
  }

  return intersectedObjects;
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

void BSPTree::exportToDot(const std::string &filename) const
{
  std::ofstream dotFile(filename);
  dotFile << "digraph BSPTree {" << std::endl;
  exportNodeToDot(root, dotFile);
  dotFile << "}" << std::endl;
}

void BSPTree::exportNodeToDot(BSPNode *node, std::ofstream &dotFile) const
{
  if (!node)
    return;

  // Ajouter le nœud courant
  dotFile << "  \"" << node << "\" [label=\"";
  dotFile << "Id: " << node->id << "\\n";
  dotFile << "Objects: " << node->objects.size() << "\\n";
  dotFile << "BBox Min: " << node->boundingBox.getMin() << "\\n";
  dotFile << "BBox Max: " << node->boundingBox.getMax();
  dotFile << "\"];" << std::endl;

  // Relier aux enfants
  if (node->leftChild)
  {
    dotFile << "  \"" << node << "\" -> \"" << node->leftChild << "\";" << std::endl;
    exportNodeToDot(node->leftChild, dotFile);
  }
  if (node->rightChild)
  {
    dotFile << "  \"" << node << "\" -> \"" << node->rightChild << "\";" << std::endl;
    exportNodeToDot(node->rightChild, dotFile);
  }
}
