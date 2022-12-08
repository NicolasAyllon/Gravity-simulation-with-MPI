#include <vector>
#include "vector.h"
#include "particle.h"
#include "physics.h"

void calc_net_force(Particle* p, QuadtreeNode* q, Vec2<double>& f) {

}

Vec2<double> calc_net_force(Particle p, Quadtree tree) {
  // TODO
  Vec2<double> force = {0,0};
  calc_net_force(&p, tree.root, force);
  // Inorder traversal

  // If
  
  return force;
}