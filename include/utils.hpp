#pragma once

#include <cstdlib>
#include <vector>

namespace std {
  double rand(const double& max);
  size_t randMod(const size_t& mod);
}

namespace jce {
  template <typename T>
  T& randElement(std::vector<T>& vec);
  template <typename T>
  const T& randElement(const std::vector<T>& vec);
}

inline double std::rand(const double& max) {
  return (double)(std::rand()) / (((double)RAND_MAX/max));
}
inline size_t std::randMod(const size_t& mod) {
  return std::rand() % mod;
}

template<typename T>
T& jce::randElement(std::vector<T>& vec) {
  return vec[std::randMod(vec.size())];
}
template<typename T>
const T& jce::randElement(const std::vector<T>& vec) {
  return vec[std::randMod(vec.size())];
}