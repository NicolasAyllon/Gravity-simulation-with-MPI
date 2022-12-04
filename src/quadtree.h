#ifndef _QUADTREE_H
#define _QUADTREE_H

#include "particle.h"
#include "vector.h"

template <typename T>
struct Region {
  T x_min;
  T x_max;
  T y_min;
  T y_max;
};

template <typename T>
struct QuadtreeNode {
  Region<T> region;       // Bounds
  QuadtreeNode* q1;       // NE
  QuadtreeNode* q2;       // NW
  QuadtreeNode* q3;       // SW
  QuadtreeNode* q4;       // SE
  
  T total_mass;           // Sum of all particle masses in this region
  Vec2<T> center_of_mass; // Position of the center of mass for this region
  Particle* particle;     // -internal node: nullptr
                          // -leaf node: pointer to particle
};

#endif // _QUADTREE_H