#include "ros/ros.h"
#include <unistd.h>
#include "following_goal/Robot_pv.h"
#include <following_goal/PlanningActionGoal.h>
#include <cmath>
#include <unistd.h>


// global variables
float current_posx, current_posy, goal_posx, goal_posy, dist_togx, dist_togy, vx, vz, aver_speed_x, aver_speed_z, sum_velx, sum_velz;
int count_v_lin=1;
int count_v_ang=1;

// boolean variable for setting, at the beginning, the two averages
bool initial_linear=true;
bool initial_angular=true;
double frequency;

// callback for subscribing current_position
void pos_v_Callback(const following_goal::Robot_pv::ConstPtr& msg) {	

	current_posx=msg->posx;
	current_posy=msg->posy;
	vx=msg->velx;
	vz=msg->velz;
}

// callback for subscribing goal position
void pos_v_goal_Callback(const following_goal::PlanningActionGoal::ConstPtr& msg) {	

	goal_posx=msg->goal.target_pose.pose.position.x;
	goal_posy=msg->goal.target_pose.pose.position.y;
}

// this function computes the eucledian distance from the goal to the current position
void current_goal_dist() {
	
	// component x of distance from goal
	dist_togx=abs (goal_posx-current_posx);
	
	// component y of distance from goal
	dist_togy=abs (goal_posy-current_posy);
	
	// compute euclidian distance
	float eucl_distance=sqrt((dist_togx*dist_togx)+(dist_togy*dist_togy));
	ROS_INFO("Distance goal pos:%f", eucl_distance);
}

// this function computes the average linear speed
void average_speed_lin() {

	// when everything starts, the average speed is equal to current speed
	if ( initial_linear) {
		sum_velx=abs(vx);
		initial_linear=false;
	}
	
	// sum all speeds up to now
	else 
		sum_velx=sum_velx+abs(vx);
	
	// compute the average
	aver_speed_x=sum_velx/count_v_lin;
	ROS_INFO("Average linear speed:%f", aver_speed_x);
	
	// increment the number of total speeds
	count_v_lin++;
}

// this function computes the average angular speed
void average_speed_ang() {

	// when everything starts, the average speed is equal to current speed
	if ( initial_angular) {
		sum_velz=abs(vz);
		initial_angular=false;
	}
	
	// sum all speeds up to now
	else 
		sum_velz=sum_velz+abs(vz);
		
	// compute the average
	aver_speed_z=sum_velz/count_v_ang;
	ROS_INFO("Average angular speed:%f", aver_speed_z);
	
	// increment the number of total speeds
	count_v_ang++;
}


int main ( int argc, char ** argv) {
	
	/* you must call one of the versions of ros::init() before using any other
  	part of the ROS system. */
	ros::init(argc, argv, "robot_subscriber");
	
	// NodeHandle is the main access point to communications with the ROS system.
	ros::NodeHandle n;
	
	// get frequency
	ros::param::get("frequency", frequency);
	
	// set the rate
	ros::Rate rate(frequency);
	
	/* this method tells how is the subscriber:
		the first argument tells the topic where it takes the information
		the second one is the size of message queue
		the third is the callback that is called each time
	*/
	ros::Subscriber sub1=n.subscribe("/info_robot", 1, pos_v_Callback);
	
	/* this method tells how is the subscriber:
		the first argument tells the topic where it takes the information
		the second one is the size of message queue
		the third is the callback that is called each time
	*/
	ros::Subscriber sub2=n.subscribe("reaching_goal/goal", 1, pos_v_goal_Callback);
	
	while (ros::ok()) {
		
		// call the function for computing distance
		current_goal_dist();
		
		// call the functions for computing average velocities
		average_speed_lin();
		average_speed_ang();
		rate.sleep();
		ros::spinOnce();
	}
	return 0;
}
