#include <iostream>
#include <cmath>
#include "Sphere.hpp"
#include "../raymath/Vector3.hpp"

Sphere::Sphere(double r) : SceneObject(), radius(r)
{
}

Sphere::~Sphere()
{
}

void Sphere::applyTransform()
{
  Vector3 c;
  this->center = this->transform.apply(c);
}

bool Sphere::intersects(Ray &r, Intersection &intersection, CullingType culling)
{
  // Pré-calculer les informations du rayon
  const Vector3 &rayOrigin = r.GetPosition();
  const Vector3 &rayDirection = r.GetDirection();

  // Vector from ray origin to center of sphere
  Vector3 OC = center - rayOrigin;

  // Project OC onto the ray
  // double tProj = OC.dot(rayDirection);
  // Project OC onto the ray
  Vector3 OP = OC.projectOn(rayDirection);

  // If the OP vector is pointing in the opposite direction of the ray
  // ... then it is behind the ray origin, ignore the object
  // if (tProj <= 0)
  // {
  //   return false;
  // }
  if (OP.dot(rayDirection) <= 0)
  {
    return false;
  }

  // Calculer la distance au carré entre le centre de la sphère et le rayon
  // double distSquared = OC.dot(OC) - (tProj * tProj);

  // P is the corner of the right-angle triangle formed by O-C-P
  Vector3 P = rayOrigin + OP;

  // Vérifier si cette distance dépasse le rayon au carré (pas d'intersection)
  // double radiusSquared = radius * radius;
  // if (distSquared > radiusSquared)
  // {
  //   return false;
  // }

  // Is the length of CP greater than the radius of the circle ? If yes, no intersection!
  Vector3 CP = P - center;
  double distanceSquared = CP.lengthSquared();
  double radiusSquared = radius * radius;
  if (distanceSquared > radiusSquared)
  {
    return false;
  }

  // Calculer la distance du point d'intersection au long du rayon
  // double thc = sqrt(radiusSquared - distSquared); // Distance de l'intersection à tProj
  // double t = tProj - thc;
  // Calculate the exact point of collision: P1
  double a = sqrt(radiusSquared - distanceSquared);
  double t = OP.length() - a;

  // Calculer le point exact d'intersection
  Vector3 P1 = rayOrigin + rayDirection * t;

  // Préparer les informations d'intersection
  intersection.Position = P1;
  intersection.Mat = this->material;
  intersection.Normal = (P1 - center).normalize();

  return true;
}
