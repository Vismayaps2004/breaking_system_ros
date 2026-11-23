#include <cmath>
#include <breaking_system_ros/Position.h>
#include <breaking_system_ros/Vehicle.h>

Position Vehicle::getPosition () const {
  return position;
}

double Vehicle::getSpeed () const {
  return speed;
}

void Vehicle::updatePosition (double time) {
  if (isVehicleStopped) {
    return;
  }
  position.x += speed * time;
}

double Vehicle::calculateDistance(const Position& obstacle) {
  double dx = position.x - obstacle.x;
  double dy = position.y - obstacle.y;

  return std::sqrt(dx * dx + dy * dy);
}

void Vehicle::applySafetyCommand(const std::string& command, double newSpeed) {
    if (command == "Stop") {
        isVehicleStopped = true;
        speed = 0;
        return;
    }

    if (isVehicleStopped) {
        return;
    }

    speed = newSpeed;
}