#include "particle.h"

// Particle::Particle() 
//     : index(0), 
//       mass(0), 
//       position(Vec2<double>()), 
//       velocity(Vec2<double>()) {};

bool coincident(Particle* a, Particle* b) {
  return a->position.x == b->position.x && 
         a->position.y == b->position.y;
}

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
  // r = r + (v + 0.5*a*dt)*dt;  // Save 1 op using by factoring out dt
  // v = v + a*dt;
}



////////////////////////////////////////////////////////////////////////////////
// String
////////////////////////////////////////////////////////////////////////////////

std::string Particle::toString() const {
  std::stringstream ss;
  ss << "Particle: [idx: "      << index  << ", "
                << "mass: "     << mass   << ", "
                << "position: " << position.toString() << ", "
                << "velocity: " << velocity.toString() << "]";
  return ss.str();
}

std::string Particle::toStringMatchInputOrder(bool show_address) const {
  std::stringstream ss;
  ss << "Particle: [idx: "      << index  << ", "
                << "position: " << position.toString() << ", "
                << "mass: "      << mass   << ", "
                << "velocity: " << velocity.toString() << "]";
  if (show_address) {
    ss << " @ " << this;
  }
  return ss.str();
}