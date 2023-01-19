#include "following_goal/NumberGoals.h"
#include "ros/ros.h"
#include "assignment_2_2022/PlanningActionResult.h"
#include "following_goal/PlanningActionGoal.h"
#include "actionlib_msgs/GoalStatus.h"


int reach=0;
int eliminate=0;
int status;

bool count (following_goal::NumberGoals::Request &req, following_goal::NumberGoals::Response &res) {
	res.reached=reach;
	res.cancelled=eliminate;
	return true;
}

void count_reach_canc_Callback (const assignment_2_2022::PlanningActionResult::ConstPtr& msg) {
	status=msg->status.status;
	//ROS_INFO("Current status goal:[%d]", status);
	if (status==3) {
		reach++;
		ROS_INFO("Current status goal:[%d]", reach);
	}
	else if ( status==2) {
		eliminate++;
		ROS_INFO("Current status goal:[%d]", eliminate);
	}
}

int main (int argc, char** argv) {

	ros::init(argc, argv, "count_goal");
	ros::NodeHandle n;
	
	//velocity is the name of identifier service; my_random is the callback function
	ros::Subscriber sub=n.subscribe("reaching_goal/result", 1, count_reach_canc_Callback);
	ros::ServiceServer service=n.advertiseService("/result", count);
	ros::spin();

}
