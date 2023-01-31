#include "following_goal/NumberGoals.h"
#include "ros/ros.h"
#include "assignment_2_2022/PlanningActionResult.h"

// global variables
int reached=0;
int eliminated=0;
int status;

/* similar to callback function, set in the custom service:
	number of reach goals;
	number of eliminated goals */
bool goal_service (following_goal::NumberGoals::Request &req, following_goal::NumberGoals::Response &res) {
	res.reached=reached;
	res.cancelled=eliminated;
	return true;
}

/* this callback takes the status of current node and counts goals*/
void count_reach_canc_Callback (const assignment_2_2022::PlanningActionResult::ConstPtr& msg) {
	
	status=msg->status.status;
	ROS_INFO("Status goal:%d", status);
	
	// check if the current goal is succeeded
	if (status==3) 
		reached++;
	
	// check if the current goal is preemptive
	else if ( status==2) 
		eliminated++;
}

int main (int argc, char** argv) {
	
	/* you must call one of the versions of ros::init() before using any other
  	part of the ROS system. */
	ros::init(argc, argv, "count_goal");
	
	// NodeHandle is the main access point to communications with the ROS system.
	ros::NodeHandle n;
	
	// define a subscriber, which takes information from topic reaching_goal/result
	ros::Subscriber sub=n.subscribe("reaching_goal/result", 1, count_reach_canc_Callback);
	
	// define a service, which send information on service result
	ros::ServiceServer service=n.advertiseService("/result", goal_service);
	sleep(1);
	ros::spin();
	return 0;

}
