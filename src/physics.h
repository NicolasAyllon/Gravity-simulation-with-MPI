#ifndef _PHYSICS_H
#define _PHYSICS_H

#include <vector>
#include "quadtree.h"
#include "vector.h"

Vec2<double> calc_net_force(const Particle& p, const Quadtree& tree, double theta);
void calc_net_force(const Particle& p, const Quadtree& tree, double theta, Vec2<double>& f);

#endif // _PHYSICS_H