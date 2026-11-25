#pragma once
#include <breaking_system_ros/Position.h>
#include <string>


class Vehicle {
  private:
    Position position{0, 0};
    double speed = 20;
    bool isVehicleStopped = false;

  public:
    Position getPosition () const;
    double getSpeed () const;
    void updatePosition (double time);
    double calculateDistance(const Position& obstacle);
    void applySafetyCommand(const std::string& command, double newSpeed);
};