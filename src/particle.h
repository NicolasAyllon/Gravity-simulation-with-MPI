#ifndef _PARTICLE_H
#define _PARTICLE_H

#include "vector.h"

struct Particle {
  int index;
  double mass;
  Vec2<double> position;
  Vec2<double> velocity;

  Particle() = default;
  // Particle();

  void update(Vec2<double> force, double dt);
  std::string toString() const;
  std::string toStringMatchInputOrder(bool) const;
};

bool coincident(Particle* a, Particle* b);

#endif