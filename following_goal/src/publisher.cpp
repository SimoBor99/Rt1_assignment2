#include "ros/ros.h"
#include <unistd.h>
#include "nav_msgs/Odometry.h"
#include "following_goal/Robot_pv.h"

// global variable
float posx, posy, vx, vy;
ros::Publisher pub;

void pos_v_Callback(const nav_msgs::Odometry::ConstPtr& msg) {	
	ROS_INFO("Robot pos and vel:[%f, %f, %f, %f]", msg->pose.pose.position.x, msg->pose.pose.position.y, msg->twist.twist.linear.x, msg->twist.twist.linear.y);
	
	/* Subscribe position and 
	velocity from topic odom */
	posx=msg->pose.pose.position.x;
	posy=msg->pose.pose.position.y;
	vx=msg->twist.twist.linear.x;
	vy=msg->twist.twist.linear.y;
	following_goal::Robot_pv pv;
	
	/* Sent position and velocity 
	on topic info_robot*/
	pv.posx=posx;
	pv.posy=posy;
	pv.velx=vx;
	pv.vely=vy;
	
	// publish the information
	pub.publish(pv);
}

int main(int argc, char ** argv) {

	/* You must call one of the versions of ros::init() before using any other
  	part of the ROS system. */
	ros::init(argc, argv, "robot_publisher");

	// NodeHandle is the main access point to communications with the ROS system.
	ros::NodeHandle n;
	
	// Call the subscriber
	ros::Subscriber sub=n.subscribe("/odom", 10, pos_v_Callback);
	
	// Call the publisher 
	pub=n.advertise<following_goal::Robot_pv>("/info_robot", 10);

	sleep(1);
	ros::spin();
	return 0;
}