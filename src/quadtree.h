#ifndef _QUADTREE_H
#define _QUADTREE_H

#include <array>
#include <iostream>
#include <sstream>
#include <string>
#include "particle.h"
#include "vector.h"

////////////////////////////////////////////////////////////////////////////////
// Quadrant & Region
////////////////////////////////////////////////////////////////////////////////

// Enumerates quadrants of a rectangular region
enum Quadrant {
  NE, 
  NW, 
  SW,
  SE, 
  None
};

// Represents a rectangular region in the xy-plane
template <typename T>
struct Region {
  T x_min;
  T x_max;
  T y_min;
  T y_max;

  T x_center() const { return (x_min + x_max)/2; }
  T y_center() const { return (y_min + y_max)/2; }
  T side_length() const { return x_max - x_min; } // Assume square
  
  Region<T> subregion(Quadrant q) {
    if (q == Quadrant::NE) return Region{x_center(), x_max, y_center(), y_max};
    if (q == Quadrant::NW) return Region{x_min, x_center(), y_center(), y_max};
    if (q == Quadrant::SW) return Region{x_min, x_center(), y_min, y_center()};
    if (q == Quadrant::SE) return Region{x_center(), x_max, y_min, y_center()};
    return Region{0, 0, 0, 0};
  }

  std::string toString() {
    std::stringstream ss;
    ss << "[ x_min: " << x_min << ", x_max: " << x_max
       << ", y_min: " << y_min << ", y_max: " << y_max << " ]";
    return ss.str();
  }
};

// Returns whether the particle is contained in the rectangular region.
// Note: A particle on an edge or corner is considered to be in the region.
template <typename T>
bool isContained(const Particle& p, const Region<T>& r) {
  return (r.x_min <= p.position.x && p.position.x <= r.x_max) &&
         (r.y_min <= p.position.y && p.position.y <= r.y_max);
}

// Returns the quadrant of the region the particle lies in.
// Note: This function does not check whether the particle is in the region,
// because this this was checked before it was inserted in the quadtree.
template <typename T>
Quadrant quadrant(const Particle& p, const Region<T>& r) {
  if (p.position.y > r.y_center()) {
    return (p.position.x < r.x_center() ? Quadrant::NW : Quadrant::NE);
  } else {
    return (p.position.x < r.x_center() ? Quadrant::SW : Quadrant::SE);
  }
}

////////////////////////////////////////////////////////////////////////////////
// QuadtreeNode
////////////////////////////////////////////////////////////////////////////////

struct QuadtreeNode {
  Region<double> region;  // Bounds
  Particle* particle;     // -internal node: nullptr
                          // -leaf node: pointer to particle
  std::array<QuadtreeNode*, 4> quadrants; // Array of pointers to child nodes

  double total_mass;  // Sum of particle masses in this region
  int num_particles;  // Number of particles in this region
  Vec2<double> com;   // Position of center of mass for this region

  // Construct a quadtree node for the given region, empty with no particles
  QuadtreeNode(Region<double> r);
  // Construct a quadtree node the region containing the 1 particle passed in
  QuadtreeNode(Region<double> r, Particle* p);
  
  std::string toString();
};

////////////////////////////////////////////////////////////////////////////////
// Quadtree
////////////////////////////////////////////////////////////////////////////////
struct Quadtree {
  const Region<double> region;
  QuadtreeNode* root;

  Quadtree(const Region<double>& region);
  Quadtree(const Quadtree&) = delete;
  ~Quadtree();
  Quadtree& operator=(const Quadtree&) = delete;

  bool insert(Particle& p);

  private: 
  QuadtreeNode* insert(QuadtreeNode*, Region<double>, Particle* p);
  void destroy(QuadtreeNode* node);
};

#endif // _QUADTREE_H