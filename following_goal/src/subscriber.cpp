#include "ros/ros.h"
#include <unistd.h>
#include "following_goal/Robot_pv.h"
#include <following_goal/PlanningActionGoal.h>
#include <cmath>
#include <unistd.h>


// global variables
float current_posx, current_posy, goal_posx, goal_posy, dist_togx, dist_togy, vx, vy;

double frequency;

// callback for subscribing current_position
void pos_v_Callback(const following_goal::Robot_pv::ConstPtr& msg) {	

	current_posx=msg->posx;
	current_posy=msg->posy;
	vx=msg->velx;
	vy=msg->vely;
}

// callback for subscribing goal position
void pos_v_goal_Callback(const following_goal::PlanningActionGoal::ConstPtr& msg) {	

	goal_posx=msg->goal.target_pose.pose.position.x;
	goal_posy=msg->goal.target_pose.pose.position.y;
}

// this function compute the eucledian distance from the goal to the current position
void current_goal_dist() {
	
	// component x of distance from goal
	dist_togx=abs (goal_posx-current_posx);
	
	// component y of distance from goal
	dist_togy=abs (goal_posy-current_posy);
	
	// compute euclidian distance
	float eucl_distance=sqrt(dist_togx*dist_togx+dist_togy*dist_togy);
	ROS_INFO("Distance goal pos:%f", eucl_distance);
}

void average_speed() {

	// compute the average speed, thanks to x and y components
	float aver_speed=sqrt((vx*vx)+(vy*vy));
	ROS_INFO("Average speed:%f", aver_speed);
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
	
	while (ros::ok()) {
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
		
		// call the function for computing distance
		current_goal_dist();
		
		// call the function for computing average velocity
		average_speed();
		rate.sleep();
		ros::spinOnce();
	}
	return 0;
}
