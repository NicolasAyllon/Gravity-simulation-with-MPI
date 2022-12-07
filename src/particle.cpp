#include "particle.h"

////////////////////////////////////////////////////////////////////////////////
// String
////////////////////////////////////////////////////////////////////////////////

std::string Particle::toString() {
  std::stringstream ss;
  ss << "Particle: [idx: "      << index  << ", "
                << "mass: "      << mass   << ", "
                << "position: " << position.toString() << ", "
                << "velocity: " << velocity.toString() << "]";
  return ss.str();
}

std::string Particle::toStringMatchInputOrder(bool show_address) {
  std::stringstream ss;
  ss << "Particle: [idx: "      << index  << ", "
                << "position: " << position.toString() << ", "
                << "mass: "      << mass   << ", "
                << "velocity: " << velocity.toString() << "]";
  if(show_address) {
    ss << " @ " << this;
  }
  return ss.str();
}