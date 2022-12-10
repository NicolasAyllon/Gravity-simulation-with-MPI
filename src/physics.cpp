#include "vector.h"
#include "particle.h"
#include "physics.h"

// Suggested constants
constexpr double r_limit = 0.03;
constexpr double G = 0.0001;

// Returns the force exerted on m1 (at position r1) by m2 (at position r2)
Vec2<double> gravity(double m1, double m2, Vec2<double> r1, Vec2<double> r2) {
  // Compute separation distance d. Use r_limit if d < r_limit.
  double d = dist(r1, r2);
  d = std::max(d, r_limit);
  // Compute force with inverse square law:
  // For m1 at position r and m2 at position r', the force (m2 on m1) is:
  // f = G*m1*m2*(r'-r) / |r'-r|^3
  return G*m1*m2*(r2-r1)/(d*d*d);
}

// Recursively traverses the quadtree in-order and writes the result in the
// output parameter f.
// 
// For each nodes containing only 1 particle or meeting the approximation
// threshold, the gravitational force (or approximation) is computed and added
// to the net force. Otherwise, the function examines the nodes below.
void calc_net_force(const Particle* p, QuadtreeNode* node, double theta,
                    Vec2<double>& f) {
  // If the node is nullptr, do nothing and return.
  if (node == nullptr) {
    return;
  }
  // If there is only 1 particle, compute force due to it and add to f.
  if (node->num_particles == 1) {
    Particle* q = node->particle;
    // A particle does not exert force on itself.
    if (q->index != p->index) {
      f += gravity(p->mass, q->mass, p->position, q->position);
    }
    return;
  }
  // If s/d < theta, approximate the force from all particles in this node
  // as that from a point mass located at the center of mass with a mass equal
  // to the total mass of all particles within.
  double s = node->region.side_length();
  double d = dist(p->position, node->com);
  if (s/d < theta) {
    f += gravity(p->mass, node->total_mass, p->position, node->com);
    return;
  }
  // Otherwise, no approximation can be made, and we need to recursively
  // examine all nodes under this one.
  calc_net_force(p, node->quadrants[Quadrant::NE], theta, f);
  calc_net_force(p, node->quadrants[Quadrant::NW], theta, f);
  calc_net_force(p, node->quadrants[Quadrant::SW], theta, f);
  calc_net_force(p, node->quadrants[Quadrant::SE], theta, f);
}

// Calculate the net force on particle p from all other particles in the
// quadtree using the given value of theta as a threshold for approximations.
// Note: force vector is returned by value
Vec2<double> calc_net_force(const Particle& p, const Quadtree& tree, double theta) {
  // Ignore lost particles
  if(p.mass == -1) return {0,0};
  // Create 0 vector to start, modify, then return
  Vec2<double> force = {0,0};
  calc_net_force(&p, tree.root, theta, force);  
  return force;
}
