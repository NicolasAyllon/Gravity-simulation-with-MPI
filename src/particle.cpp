#include "particle.h"

////////////////////////////////////////////////////////////////////////////////
// Physics update
////////////////////////////////////////////////////////////////////////////////
void Particle::update(Vec2<double> force, double dt) {
  Vec2<double> acceleration = force/mass;
  auto& a = acceleration;
  auto& v = velocity;
  auto& r = position;
  // Update position & velocity (Method 1: 8 ops)
  r = r + v*dt + 0.5*a*dt*dt;
  v = v + a*dt;
  // Update position & velocity (Method 2: 7 ops)
  // r = r + (v + 0.5*a*dt)*dt;  // Alternate with 5 ops instead of 6
  // v = v + a*dt;
}



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