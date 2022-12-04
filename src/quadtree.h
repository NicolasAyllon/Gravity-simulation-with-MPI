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

  T total_mass;           // Sum of particle masses in this region
  int num_particles;      // Number of particles in this region
  Vec2<T> center_of_mass; // Position of the center of mass for this region
  Particle* particle;     // -internal node: nullptr
                          // -leaf node: pointer to particle

  // Construct a quadtree node for the given region, empty with no particles
  QuadtreeNode(Region r) 
      : region(r), 
        q1(nullptr), 
        q2(nullptr), 
        q3(nullptr), 
        q4(nullptr), 
        total_mass(T()),            // default-construct: 0 for numeric
        num_particles(0),
        center_of_mass(Vec2<T>()),  // default-construct: (0,0) for numeric
        particle(nullptr) {}

    // [?] Add constructor for quadtree node containing particle?
};


template <typename T>
struct BarnesHutTree {
  *QuadtreeNode root;

  BarnesHutTree(const Region<T>& region);
  bool insert(const Particle& p);
};

template <typename T>
BarnesHutTree<T>::BarnesHutTree(const Region<T>& region) 
    : root(new QuadtreeNode(region)) {}

template <typename T>
bool BarnesHutTree<T>::insert(const Particle& p) {
  // TODO
}


#endif // _QUADTREE_H