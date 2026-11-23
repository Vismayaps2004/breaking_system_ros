#pragma once
#include <breaking_system_ros/Position.h>

class Vehicle {
  private:
    Position position{30, 100};
    double speed = 20;

  public:
    Position getPosition () const;
    double getSpeed () const;
    void updatePosition (double time);
    double calculateDistance(const Position& obstacle);
    void updateSpeed(double speed);
};