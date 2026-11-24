#include <ros/ros.h>
#include <geometry_msgs/Point.h>
#include <breaking_system_ros/Vehicle.h>
#include <breaking_system_ros/Position.h>
#include <breaking_system_ros/vehicle_status.h>
#include <breaking_system_ros/status.h>

void publishPosition (ros::Publisher pub, const Vehicle& vh) {
  Position position = vh.getPosition();
  geometry_msgs::Point msg;
  msg.x = position.x;
  msg.y = position.y;
  msg.z = 0;

  pub.publish(msg);
}

void applySafetyCommand (const breaking_system_ros::status::ConstPtr& msg, Vehicle& vh) {
  vh.applySafetyCommand(msg->command, msg->speed);
}

int main (int argc, char **argv) {
  ros::init(argc, argv, "vehicle_node");
  ros::NodeHandle nh;
  Vehicle vh;
  ros::Rate rate(10);

  ros::Publisher pub = nh.advertise<geometry_msgs::Point>("/vehicle_position", 10);
  ros::Time prevTime = ros::Time::now();

  XmlRpc::XmlRpcValue obstacle;
  nh.getParam("/obstacle_coordinates", obstacle);

  Position obstacleCoord {
    static_cast<double>(obstacle["x"]),
    static_cast<double>(obstacle["y"])
  };

  ros::Publisher statusPub = nh.advertise<breaking_system_ros::vehicle_status>("/vehicle_status", 10);
   ros::Subscriber statusSub = nh.subscribe<breaking_system_ros::status>(
      "/status_command",
      10,
      [&vh](const breaking_system_ros::status::ConstPtr& msg){
        applySafetyCommand(msg, vh);
      });
  while (ros::ok()) {
    ros::Time currTime = ros::Time::now();
    double deltaTime = (currTime - prevTime).toSec();
    prevTime = currTime;

    vh.updatePosition(deltaTime);
    publishPosition(pub, vh);
    double distance = vh.calculateDistance(obstacleCoord);

    breaking_system_ros::vehicle_status msg;
    msg.distance = distance;
    msg.speed = vh.getSpeed();
    statusPub.publish(msg);
 
    ros::spinOnce();
    rate.sleep();
  }
}