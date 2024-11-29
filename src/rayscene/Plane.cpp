#include <iostream>
#include <cmath>
#include "Plane.hpp"
#include "../raymath/Vector3.hpp"

Plane::Plane(Vector3 p, Vector3 n) : point(p), normal(n)
{
}

Plane::~Plane()
{
}

void Plane::applyTransform()
{
#ifdef ENABLE_BOUNDING_BOX
  calculateBoundingBox();
#endif
}

bool Plane::intersects(Ray &r, Intersection &intersection, CullingType culling)
{
  // Vérifie l'AABB pour exclure l'objet si le rayon ne l'intersecte pas
#ifdef ENABLE_BOUNDING_BOX
  if (!boundingBox.intersects(r))
  {
    return false;
  }
#endif

  float denom = r.GetDirection().dot(normal);

  // If denom == 0 - it is parallel to the plane
  // If denom > 0, it means plane is behind the ray
  if (denom > -0.000001)
  {
    return false;
  }

  float numer = (point - r.GetPosition()).dot(normal);
  float t = numer / denom;

  intersection.Position = r.GetPosition() + (r.GetDirection() * t);
  intersection.Normal = normal;
  intersection.Mat = this->material;

  return true;
}

void Plane::calculateBoundingBox()
{
  std::cout << "Calculating bounding box for Plane" << std::endl;
  float extent = 1e1f; // Une très grande valeur pour approximer l'infini
  Vector3 min = Vector3(-extent, point.y - COMPARE_ERROR_CONSTANT, -0.90);
  Vector3 max = Vector3(extent, point.y + COMPARE_ERROR_CONSTANT, extent);
  boundingBox = AABB(min, max);
}
