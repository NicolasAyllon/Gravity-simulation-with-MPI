#ifndef _QUADTREE_H
#define _QUADTREE_H

#include "particle.h"
#include "vector.h"
#include <array>
#include <iostream>
#include <sstream>
#include <string>

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
  QuadtreeNode(Region<double> r) 
      : region(r), 
        particle(nullptr),
        quadrants({nullptr, nullptr, nullptr, nullptr}), 
        total_mass(0), // default-construct: 0 for numeric
        num_particles(0),
        com(Vec2<double>(0, 0)) 
  {
    // std::cout << "Constructing quadtree node, empty:\n";
    // std::cout << toString();
  }

  // Construct a quadtree node the region containing the 1 particle passed in
  QuadtreeNode(Region<double> r, Particle* p)
      : region(r),
        particle(p),
        quadrants({nullptr, nullptr, nullptr, nullptr}),
        total_mass(p->mass),
        num_particles(1),
        com(p->position)
  {
    // std::cout << "Constructing quadtree node, with particle:\n";
    // std::cout << toString();
  }
  
  std::string toString() {
    std::stringstream ss;
    ss << "This node: "     << this                           << '\n';
    ss << "Region: "        << region.toString()              << '\n';
    ss << "particle: "      << particle                       << '\n';
    ss << "quadrants: ["    << quadrants[Quadrant::NE] << ", "
                            << quadrants[Quadrant::NW] << ", "
                            << quadrants[Quadrant::SW] << ", "
                            << quadrants[Quadrant::SE] << "]" << '\n';
    ss << "total_mass: "    << total_mass                     << '\n';
    ss << "num_particles: " << num_particles                  << '\n';
    ss << "com: "           << com.toString()                 << '\n';
    return ss.str();
  }
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

Quadtree::Quadtree(const Region<double>& r) 
    : region(r), 
      root(nullptr) {}

Quadtree::~Quadtree() {
  destroy(root);
}

void Quadtree::destroy(QuadtreeNode* root) {
  if (root == nullptr) return;
  destroy(root->quadrants[Quadrant::NE]);
  destroy(root->quadrants[Quadrant::NW]);
  destroy(root->quadrants[Quadrant::SW]);
  destroy(root->quadrants[Quadrant::SE]);
  delete root;
}

// Inserts the particle into the Quadtree
// Returns: 
//   true if the particle was inside the bounds and inserted, or
//   false if the particle is not in the bounds (also sets mass = -1)
bool Quadtree::insert(Particle& p) {
  // If particle p is inside the root region, insert it into the quadtree
  if (isContained(p, region)) {
    // Use private helping insert method
    root = insert(root, region, &p);
    return true;
  }
  // If particle p is outside the region, set mass to -1 and do not insert.
  else {
    p.mass = -1;
    return false;
  }
}

// Recursively inserts the particle into the Quadtree, starting at the 
// given node (root) and corresponding to the region passed in.
// 
// Note: Each QuadtreeNode contains its region as a member, but insert uses
// the region as a parameter on the call stack, to be available for
// constructing a new node when root == nullptr.
QuadtreeNode* Quadtree::insert(QuadtreeNode* root, 
                               Region<double> region, 
                               Particle* p) {
  
  // std::cout << "\nFrame:" << '\n';
  // std::cout << (root != nullptr ? root->toString() : "nullptr\n");
  // std::cout << "args: " << region.toString() << ", "
  //                    << p->toStringMatchInputOrder(true) << '\n'; // address
  // std::cin.get(); // wait for keypress
  // If node is null, create new node for this region containing the particle
  if (root == nullptr) {
    // std::cout << "Node* is null, need new node...\n";
    return new QuadtreeNode(region, p); 
  };

  // Internal node (contains no particles directly) or newly empty leaf node
  if (root->particle == nullptr) {
    // std::cout << "Internal node, update center of mass...\n";
    // Update center of mass (com)
    auto n = (root->com)*(root->total_mass) + (p->mass)*(p->position);
    auto d = root->total_mass + p->mass;
    root->com = n/d;
    // std::cout << "new com: " << root->com.toString() << '\n';
    // Update number of particles & total mass
    root->num_particles++;
    // std::cout << "incremented num_particles to " << root->num_particles << '\n';
    root->total_mass += p->mass;
    // std::cout << "increased mass to " << root->total_mass << '\n';
    // Insert into appropriate quadrant
    Quadrant q = quadrant(*p, region);
    // std::cout << "inserting " << p->toStringMatchInputOrder(true) << " in quadrant " << q << '\n';
    root->quadrants[q] = insert(root->quadrants[q], region.subregion(q), p);
    return root;
  }

  // Leaf node (already contains particle)
  else { // root->particle != nullptr
    // std::cout << "Leaf node, need to remove particle here and reinsert both\n";
    // If particles have same position, no amount of zoom will separate them.
    // Do not add the coincident particle and return this node unchanged.
    if (coincident(p, root->particle)) { return root; }

    // Save particle that was here & remove it
    Particle* prev = root->particle;
    root->particle = nullptr;
    // Reset fields
    root->total_mass = 0;
    root->num_particles = 0;
    root->com = {0, 0};
    // Re-insert both particles starting at this node
    root = insert(root, region, prev);
    root = insert(root, region, p);
    return root;
  }
}

#endif // _QUADTREE_H