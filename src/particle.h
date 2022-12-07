#ifndef _PARTICLE_H
#define _PARTICLE_H

#include "vector.h"

struct Particle {
  int index;
  double mass;
  Vec2<double> position;
  Vec2<double> velocity;

  void update(Vec2<double> force, double dt);
  std::string toString();
  std::string toStringMatchInputOrder(bool);
};

#endif