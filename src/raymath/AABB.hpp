#pragma once
#include "../raymath/Vector3.hpp"
#include "../raymath/Ray.hpp"
class AABB
{
private:
  Vector3 Min;
  Vector3 Max;

public:
  AABB();
  AABB(Vector3 min, Vector3 max);
  ~AABB();
  AABB &operator=(AABB const &vec);

  /**
   * Grows the AABB to include the one passed as a parameter.
   */
  void subsume(AABB const &other);

  /**
   * Splits the AABB into two along the specified axis and returns them.
   * @param axis - The axis to split along (X, Y, Z).
   * @return A pair of AABB representing the two halves.
   */
  std::pair<AABB, AABB> split(int axis) const;

  /**
   * Checks if the AABB overlaps with another AABB.
   * @param other - The other AABB to check against.
   * @return True if the AABBs overlap, false otherwise.
   */
  bool overlaps(AABB const &other);

  bool intersects(Ray &r);

  Vector3 getMin() const;
  Vector3 getMax() const;

  friend std::ostream &operator<<(std::ostream &_stream, AABB const &box);
};
