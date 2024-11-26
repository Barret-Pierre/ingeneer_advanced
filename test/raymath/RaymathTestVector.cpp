#include <iostream>
#include <cassert>
#include "../../src/raymath/Vector3.hpp"

void test_operator_plus1()
{
  Vector3 vec1(1.0, 2.0, 3.0);
  Vector3 vec2(4.0, 5.0, 6.0);
  Vector3 result = vec1 + vec2;
  assert(result.x == 5.0);
  assert(result.y == 7.0);
  assert(result.z == 9.0);
}

void test_operator_plus2()
{
  Vector3 vec3(-1.0, -2.0, -3.0);
  Vector3 vec4(1.0, 2.0, 3.0);
  Vector3 result = vec3 + vec4;
  assert(result.x == 0.0);
  assert(result.y == 0.0);
  assert(result.z == 0.0);
}

int main()
{
  test_operator_plus1();
  test_operator_plus2();
  std::cout << "All tests passed for operator+." << std::endl;
  return 0;
}