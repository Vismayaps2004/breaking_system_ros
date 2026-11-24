#include <cmath>
#include <string>
#include <ros/ros.h>
#include <breaking_system_ros/vehicle_status.h>
#include <breaking_system_ros/status.h>

enum SafetyCommands {
  CONTINUE,
  SLOW,
  STOP
};

double calculateStoppingDistance(double speed) {
    const double brakingDeceleration = 5.0;
    const double reactionTime = 0.5;

    double reactionDistance = speed * reactionTime;
    double brakingDistance =
        (speed * speed) / (2 * brakingDeceleration);

    return reactionDistance + brakingDistance;  
}

SafetyCommands determineSafetyCommand(double distance) {
  double stoppingDistance = 50;
  double slowDistance = stoppingDistance * 2;
  if (distance > slowDistance) {
    return CONTINUE;
  }

  if (distance > stoppingDistance) {
    return SLOW;
  }

    return STOP;
}

double calculateSpeed(SafetyCommands command, double distance, double speed) {
    double stoppingDistance = 50;
    double slowDistance = stoppingDistance * 2;
    switch (command) {
    case CONTINUE :
      return speed;
    case SLOW :
      return speed * (distance - stoppingDistance) / (slowDistance - stoppingDistance);
    case STOP :
      return 0;
  }
  return 0;
}

std::string getCommand (SafetyCommands command) {
  switch (command) {
    case CONTINUE :
      return "Continue";
    case SLOW :
      return "Slow";
    case STOP :
      return "Stop";
  }
  return "Stop";
}

void vehicleStatusCallback (
  const breaking_system_ros::vehicle_status::ConstPtr& msg,
  ros::Publisher& safetyPub) 
{
  double distance = msg->distance;
  double speed = msg->speed;

  SafetyCommands safetyCommand = determineSafetyCommand(distance);
  std::string command = getCommand(safetyCommand);

  double dspeed = calculateSpeed(safetyCommand, distance, speed);

ROS_INFO(
    "Distance: %.2f | Speed: %.2f | Command: %s",
    distance,
    speed,
    command.c_str()
);

  breaking_system_ros::status status;
  status.command = command;
  status.speed = dspeed;
  safetyPub.publish(status);
}

int main (int argc, char **argv) {
  ros::init(argc, argv, "safety_node");
  ros::NodeHandle nh;

  ros::Publisher safetyPub = nh.advertise<breaking_system_ros::status>("/status_command", 10);

  ros::Subscriber statusSub = nh.subscribe<breaking_system_ros::vehicle_status>("/vehicle_status", 10, 
    [&safetyPub](const breaking_system_ros::vehicle_status::ConstPtr& msg) {
        vehicleStatusCallback(msg, safetyPub);
    });
  ros::spin();
}