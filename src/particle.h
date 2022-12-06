#ifndef _PARTICLE_H
#define _PARTICLE_H

#include "vector.h"

struct Particle {
  int index;
  double mass;
  Vec2<double> position;
  Vec2<double> velocity;

  std::string toString() {
    std::stringstream ss;
    ss << "Particle: [idx: "      << index  << ", "
                  << "mass:"      << mass   << ", "
                  << "position: " << position.toString() << ", "
                  << "velocity: " << velocity.toString() << "]";
    return ss.str();
  }
};

#endif