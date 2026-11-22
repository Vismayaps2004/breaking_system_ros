#include <breaking_system_ros/Position.h>
#include <breaking_system_ros/Vehicle.h>

Position Vehicle::getPosition () const {
  return position;
}

double Vehicle::getSpeed () const {
  return speed;
}

void Vehicle::updatePosition (double time) {
  position.x = position.x + (getSpeed() * time);
  position.y = position.y + (getSpeed() * time);
}
