#include <ros/ros.h>
#include <breaking_system_ros/Vehicle.h>

int main (int argc, char **argv) {
  ros::init(argc, argv, "/vehicle_node");
  NodeHanle nh;
  Vehicle vh;
  ros::Rate rate(10);

  Publisher pub = nh.publish()

  ros::Time prevTime = ros::Time::now().toSec();
  while (ros::ok()) {
    ros::Time currTime = ros::Time::now().toSec();
    double deltaTime = currTime - prevTime;
    prevTime = currTime;

    vh.updatePosition(deltaTime);
    vh.publishPosition();
    rate.sleep();
  }
}