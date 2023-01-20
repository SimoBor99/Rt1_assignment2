#include "ros/ros.h"
#include <unistd.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <following_goal/PlanningAction.h>
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/Point.h"
#include "following_goal/NumberGoals.h"
#include <string>
#include <iostream>

float posx, posy;
int reach, eliminate;
ros::ServiceClient client;
following_goal::NumberGoals srv1;


int input (char answer) {
	std::cout<<"Options:\n 1) Set goal;\n 2) Cancell current goal;\n 3) Number of cancelled or reached goal; \n 4) Exit\n Choose one of it\n";
  	std::cin>>answer;
  	return answer;
}

void number_goals() {
	client.waitForExistence();
    	client.call(srv1);
    	reach=srv1.response.reached;
    	eliminate=srv1.response.cancelled;
    	ROS_INFO("Number of reached and eliminated goals:[%d, %d]", reach, eliminate);
}



int main (int argc, char ** argv) {

	ros::init(argc, argv, "set_goal");
	ros::NodeHandle n;
	
	// create the action client
	// true causes the client to spin its own thread
	actionlib::SimpleActionClient<following_goal::PlanningAction> ac("/reaching_goal", true);
	actionlib::SimpleClientGoalState state=ac.getState();
  	char answer;
  	int set_goal=0;
  	int res;
  	following_goal::PlanningGoal goal;
  	ROS_INFO("Waiting for action server to start.");
  	
  	// wait for the action server to start
  	ac.waitForServer(); //will wait for infinite time
	while (ros::ok()) {
  		answer=input(answer);
  		state=ac.getState();
  		res=state.toString().compare("SUCCEEDED");
  		if (res==0 && set_goal)
  			set_goal--;
  		switch (answer) {
  		case '1':
  			state=ac.getState();
  			res=state.toString().compare("SUCCEEDED");
  			if (set_goal && res!=0) 
  				std::cout<<"You have to delete a goal before setting a new one\n";
  				
  			if (res==0 && set_goal)
  				set_goal--;
  			
  			else if (!set_goal) {
  				std::cout<<"Insert x, y position\n";
				std::cin>>posx>>posy;
				std::cout<<"Goal sent\n";
				goal.target_pose.pose.position.x=posx;
				goal.target_pose.pose.position.y=posy;
				ac.sendGoal(goal);
				set_goal++;
			}
			sleep(1);
			break;
		case '2':
			state=ac.getState();
  			res=state.toString().compare("SUCCEEDED");
			if (set_goal && res!=0 ) {
				ac.cancelGoal();
				ROS_INFO("Goal has been cancelled\n");
				set_goal--;
			}
			else 
				std::cout<<"All goals have been cancelled: set new one, or exit\n";
			sleep(1);
			break;
		
    		case '3':
    			client=n.serviceClient<following_goal::NumberGoals>("/result");
    			number_goals();
    			sleep(1);
    			break;
    			
    		case '4':
    			if (set_goal)
    				ac.cancelGoal();
    			exit(0);
    			break;
    		
		default:
			std::cout<<"Choose something in the menu\n";
			sleep(1);
		}
			
	}
  	return 0;
}
