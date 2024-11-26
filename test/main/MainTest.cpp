#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <openssl/sha.h>
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

void compareHashImages(size_t outputImageHash, const std::vector<unsigned char> &outputImageBytes, size_t referenceImageHash, const std::vector<unsigned char> &referenceImageBytes)
{
  printf("Reference image hash size: %zu\n", referenceImageBytes.size());
  printf("Reference image hash: %ld\n", referenceImageHash);
  printf("Image hash size: %zu\n", outputImageBytes.size());
  printf("Image hash: %ld\n", outputImageHash);
}

int main(int argc, char *argv[])
{

  if (argc != 3)
  {
    std::cerr << "Usage: " << argv[0] << " <scenePath> <referenceImagePath>" << std::endl;
    return 1;
  }

  // Chemins des images
  std::string scenePath = argv[1];
  std::string referenceImagePath = argv[2];
  std::string outputImagepath = "/app/test/main/test_output.png";

  auto [scene, camera, image] = SceneLoader::Load(scenePath);

  // Exécuter l'application pour générer l'image
  camera->render(*image, *scene);
  image->writeFile(outputImagepath);

  // Decode image
  std::vector<unsigned char> outputImageBytes = readFileBytes(outputImagepath);
  std::vector<unsigned char> referenceImageBytes = readFileBytes(referenceImagePath);

  // Generate hash
  size_t outputImageHash = generateHash(outputImageBytes);
  size_t referenceImageHash = generateHash(referenceImageBytes);

  // compare hash
  compareHashImages(outputImageHash, outputImageBytes, referenceImageHash, referenceImageBytes);

  // Comparer les images
  if (outputImageHash == referenceImageHash)
  {
    std::cout << "Test passed: Images are identical." << std::endl;
    delete scene;
    delete camera;
    delete image;
    return 0;
  }
  else
  {
    std::cerr << "Test failed: Images are different." << std::endl;
    delete scene;
    delete camera;
    delete image;
    return 1;
  }
}