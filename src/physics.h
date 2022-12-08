#ifndef _PHYSICS_H
#define _PHYSICS_H

#include <vector>
#include "quadtree.h"
#include "vector.h"

std::vector<Vec2<double>> calculate_forces(Particle, Quadtree);

#endif // _PHYSICS_H