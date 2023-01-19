#include "ros/ros.h"
#include <unistd.h>
#include "following_goal/Robot_pv.h"
#include <following_goal/PlanningActionGoal.h>
#include <cmath>
#include <unistd.h>


// global variables
float current_posx, current_posy, goal_posx, goal_posy, dist_togx, dist_togy, vx, vy;

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
	
	dist_togx=abs (goal_posx-current_posx);
	dist_togy=abs (goal_posy-current_posy);
	ROS_INFO("Distance goal pos:[%f, %f]", dist_togx, dist_togy);
}

void average_speed() {
	float aver_speed=sqrt((vx*vx)+(vy*vy));
	ROS_INFO("Distance average speed:[%f]", aver_speed);
}


int main ( int argc, char ** argv) {

	ros::init(argc, argv, "robot_subscriber");
	ros::NodeHandle n;
	while (ros::ok()) {
		/*this function tells how is the subscriber:
			the first argument tells the topic where it takes the information
			the second one is the size of message queue
			the third is the callback that is called each time
		*/
		ros::Subscriber sub1=n.subscribe("/info_robot", 1, pos_v_Callback);
		ros::Subscriber sub2=n.subscribe("reaching_goal/goal", 1, pos_v_goal_Callback);
		
		// call the function for compute distance
		current_goal_dist();
		ros::Rate rate(1);
		average_speed();
		ros::Rate rate_2(1);
		rate.sleep();
		ros::spinOnce();
	}
	return 0;
}
