#include <breaking_system_ros/Position.h>
#include <breaking_system_ros/Vehicle.h>
#include <cmath>

Position Vehicle::getPosition () const {
  return position;
}

double Vehicle::getSpeed () const {
  return speed;
}

void Vehicle::updatePosition (double time) {
  position.x += speed * time;
}

double Vehicle::calculateDistance(const Position& obstacle) {
  double dx = position.x - obstacle.x;
  double dy = position.y - obstacle.y;

  return std::sqrt(dx * dx + dy * dy);
}

void Vehicle::updateSpeed(double newSpeed) {
  speed = newSpeed;
}