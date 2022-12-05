#ifndef _QUADTREE_H
#define _QUADTREE_H

#include "particle.h"
#include "vector.h"
#include <array>

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

  T x_center() { return (x_min + x_max)/2; }
  T y_center() { return (y_min + y_max)/2; }
  
  Region<T> subregion(Quadrant q) {
    if (q == Quadrant::NE) return Region{x_center(), x_max, y_center(), y_max};
    if (q == Quadrant::NW) return Region{x_min, x_center(), y_center(), y_max};
    if (q == Quadrant::SW) return Region{x_min, x_center(), y_min, y_center()};
    if (q == Quadrant::SE) return Region{x_center(), x_max, y_min, y_center()};
    return Region{0,0,0,0};
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
  if(p.position.y > r.y_center()) {
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
  std::array<QuadtreeNode*, 4> quadrants;

  double total_mass;            // Sum of particle masses in this region
  int num_particles;            // Number of particles in this region
  Vec2<double> center_of_mass;  // Position of center of mass for this region
  Particle* particle;           // -internal node: nullptr
                                // -leaf node: pointer to particle

  // Construct a quadtree node for the given region, empty with no particles
  QuadtreeNode(Region<double> r) 
      : region(r), 
        quadrants({nullptr, nullptr, nullptr, nullptr}), 
        total_mass(0), // default-construct: 0 for numeric
        num_particles(0),
        center_of_mass(Vec2<double>(0, 0)),
        particle(nullptr) {}

  // Construct a quadtree node the region containing the 1 particle passed in
  QuadtreeNode(Region<double> r, Particle* p)
      : region(r),
        quadrants({nullptr, nullptr, nullptr, nullptr}),
        total_mass(p->mass),
        num_particles(1),
        center_of_mass(p->position),
        particle(p) {}
};

QuadtreeNode* insert(QuadtreeNode* root, Region<double> region, Particle* p) {
  // If node is null, create new node for this region containing the particle
  if(root == nullptr) { return new QuadtreeNode(region, p); };

  // Internal node (contains no particles directly) or newly empty leaf node
  if(root->particle == nullptr) {
    // Update center of mass (com)
    Vec2<double> com_numerator 
      = (root->center_of_mass)*(root->total_mass) + (p->mass)*(p->position);
    double com_denominator = root->total_mass + p->mass;
    root->center_of_mass = com_numerator/com_denominator;
    // Update number of particles & total mass
    root->num_particles++;
    root->total_mass += p->mass;
    // Insert into appropriate quadrant
    Quadrant q = quadrant(*p, region);
    root = insert(root->quadrants[q], root->region.subregion(q), p);
    return root;
  }

  // Leaf node (contains particle)
  else { // root->particle != nullptr
    // Save particle that was here & remove it
    Particle* prev = root->particle;
    root->particle = nullptr;
    // Reset fields
    root->total_mass = 0;
    root->num_particles = 0;
    root->center_of_mass = {0, 0};
    // Re-insert both particles starting at this node
    root = insert(root, region, prev);
    root = insert(root, region, p);
    return root;
  }
}

void destroy(QuadtreeNode* root) {
  if (root == nullptr) return;
  destroy(root->quadrants[Quadrant::NE]);
  destroy(root->quadrants[Quadrant::NW]);
  destroy(root->quadrants[Quadrant::SW]);
  destroy(root->quadrants[Quadrant::SE]);
  delete root;
}

////////////////////////////////////////////////////////////////////////////////
// Barnes Hut Tree
////////////////////////////////////////////////////////////////////////////////
struct BarnesHutTree {
  const Region<double> region;
  QuadtreeNode* root;

  BarnesHutTree(const Region<double>& region);
  BarnesHutTree(const BarnesHutTree&) = delete;
  BarnesHutTree& operator=(const BarnesHutTree&) = delete;
  ~BarnesHutTree();

  bool insert(Particle& p);
};

BarnesHutTree::BarnesHutTree(const Region<double>& r) : region(r) {}

BarnesHutTree::~BarnesHutTree() {
  destroy(root);
}

// Inserts the particle into the Quadtree
// Returns: 
//   true if the particle was inside the bounds and inserted, or
//   false if the particle is not in the bounds (also sets mass = -1)
bool BarnesHutTree::insert(Particle& p) {
  // If particle p is inside the root region, insert it into the quadtree
  if(isContained(p, region)) {
    // Use global ::insert, not member insert (this.insert(...))
    root = ::insert(root, region, &p);
    return true;
  }
  // If particle p is outside the region, set mass to -1 and do not insert.
  else {
    p.mass = -1;
    return false;
  }
}

#endif // _QUADTREE_H