#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include "../../src/rayscene/SceneLoader.hpp"
#include "../../src/lodepng/lodepng.h"

std::vector<unsigned char> readFileBytes(const std::string &filePath)
{
  std::vector<unsigned char> imageData;
  unsigned width, height;
  if (unsigned error = lodepng::decode(imageData, width, height, filePath))
  {
    std::cerr << "Erreur lors du chargement de " << filePath << ": " << lodepng_error_text(error) << std::endl;
  }
  return imageData;
}

size_t generateHash(const std::vector<unsigned char> &imageData)
{
  return std::hash<std::string>()(std::string(imageData.begin(), imageData.end()));
}

int main(int argc, char *argv[])
{

  if (argc != 3)
  {
    std::cerr << "Usage: " << argv[0] << " <scenePath> <referenceImageHash>" << std::endl;
    return 1;
  }

  // Chemins des images
  std::string scenePath = argv[1];
  size_t referenceImageHash = std::stoul(argv[2]);
  std::string outputImagepath = "/app/test/main/test_output.png";

  auto [scene, camera, image] = SceneLoader::Load(scenePath);

  // Exécuter l'application pour générer l'image
  camera->render(*image, *scene);
  image->writeFile(outputImagepath);

  // Decode image
  std::vector<unsigned char> outputImageBytes = readFileBytes(outputImagepath);

  // Generate hash
  size_t outputImageHash = generateHash(outputImageBytes);

  printf("Output image hash: %zu\n", outputImageHash);
  printf("Reference image hash: %zu\n", referenceImageHash);

  delete scene;
  delete camera;
  delete image;

  // Comparer les hash des images
  if (outputImageHash == referenceImageHash)
  {
    std::cout << "Test passed: Images are identical." << std::endl;
    return 0;
  }
  else
  {
    std::cerr << "Test failed: Images are different." << std::endl;
    return 1;
  }
}