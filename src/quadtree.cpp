#include "quadtree.h"

////////////////////////////////////////////////////////////////////////////////
// QuadtreeNode
////////////////////////////////////////////////////////////////////////////////
QuadtreeNode::QuadtreeNode(Region<double> r) 
      : region(r), 
        particle(nullptr),
        quadrants({nullptr, nullptr, nullptr, nullptr}), 
        total_mass(0), // default-construct: 0 for numeric
        num_particles(0),
        com(Vec2<double>(0, 0)) 
        {}

QuadtreeNode::QuadtreeNode(Region<double> r, Particle* p)
      : region(r),
        particle(p),
        quadrants({nullptr, nullptr, nullptr, nullptr}),
        total_mass(p->mass),
        num_particles(1),
        com(p->position)
        {}

std::string QuadtreeNode::toString() {
    std::stringstream ss;
    ss << "@: "             << this                           << ", "
       << "Region: "        << region.toString()              << ", "
       << "particle: "      << (particle != nullptr ?       
                                particle->toStringMatchInput(true) : 
                                "nullptr") << ", "
       << "quadrants: ["    << quadrants[Quadrant::NE] << ", "
                            << quadrants[Quadrant::NW] << ", "
                            << quadrants[Quadrant::SW] << ", "
                            << quadrants[Quadrant::SE] << "]" << ", "
       << "total_mass: "    << total_mass                     << ", "
       << "num_particles: " << num_particles                  << ", "
       << "com: "           << com.toString();
    return ss.str();
}

////////////////////////////////////////////////////////////////////////////////
// Quadtree
////////////////////////////////////////////////////////////////////////////////
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
  // If particle p is outside the region, it is lost.
  // Set mass to -1 and do not insert.
  else {
    if(p.mass != -1) {
      p.mass = -1;
      std::cout << "[Lost]: " << p.toString() << " Setting m = -1\n";
    }
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
  // If node is null, create new node for this region containing the particle
  if (root == nullptr) {
    return new QuadtreeNode(region, p); 
  };

  // Internal node (contains no particles directly) or newly empty leaf node
  if (root->particle == nullptr) {
    // Update center of mass (com)
    auto n = (root->com)*(root->total_mass) + (p->mass)*(p->position);
    auto d = root->total_mass + p->mass;
    root->com = n/d;
    // Update number of particles & total mass
    root->num_particles++;
    root->total_mass += p->mass;
    // Insert into appropriate quadrant
    Quadrant q = quadrant(*p, region);
    root->quadrants[q] = insert(root->quadrants[q], region.subregion(q), p);
    return root;
  }

  // Leaf node (already contains particle)
  else { // root->particle != nullptr
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
