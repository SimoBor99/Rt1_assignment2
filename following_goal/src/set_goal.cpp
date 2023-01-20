#include "ros/ros.h"
#include <unistd.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <following_goal/PlanningAction.h>
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/Point.h"
#include "following_goal/NumberGoals.h"
#include <string>

// global variables
float posx, posy;
int reach, eliminate;

// istance client and custome service
ros::ServiceClient client;
following_goal::NumberGoals srv1;

// this function takes and return the user option by the menu
char input (char answer) {
	std::cout<<"Options:\n 1) Set goal;\n 2) Cancell current goal;\n 3) Number of cancelled or reached goal; \n 4) Exit\n Choose one of it\n";
  	std::cin>>answer;
  	return answer;
}

// this function calls the custome service, and takes the number of goals
void number_goals() {
	client.waitForExistence();
    	client.call(srv1);
    	
    	// peek reached goals
    	reach=srv1.response.reached;
    	
    	// peek eliminate goals
    	eliminate=srv1.response.cancelled;
    	ROS_INFO("Number of reached and eliminated goals:[%d, %d]", reach, eliminate);
}



int main (int argc, char ** argv) {
	
	/* you must call one of the versions of ros::init() before using any other
  	part of the ROS system. */
	ros::init(argc, argv, "set_goal");
	
	// NodeHandle is the main access point to communications with the ROS system.
	ros::NodeHandle n;
	
	// create the action client
	actionlib::SimpleActionClient<following_goal::PlanningAction> ac("/reaching_goal", true);
  	
  	char answer;
  	
  	// variable stands for if a goal is set
  	int set_goal=0;
  	following_goal::PlanningGoal goal;
  	ROS_INFO("Waiting for action server to start.");
  	
  	/* wait for the action server to start; 
  	it will wait for infinite time*/
  	ac.waitForServer();
	
	while (ros::ok()) {
		
		// get user input
  		answer=input(answer);
  		
  		// get the state information of current goal
  		actionlib::SimpleClientGoalState state=ac.getState();
  		
  		// check if, when the user does nothing on the menu, the goal is reached
  		if (!state.toString().compare("SUCCEEDED") && set_goal)
  			set_goal--;
  			
  		switch (answer) {
  			case '1':
  				state=ac.getState();
  			
  				// check if a goal has been already set
  				if (set_goal && state.toString().compare("SUCCEEDED")) 
  					std::cout<<"You have to delete a goal before setting a new one\n";
  			
  				// check if a goal has reached
  				if (!state.toString().compare("SUCCEEDED") && set_goal)
  					set_goal--;
  				// check if no goal has been set
  				else if (!set_goal) {
  					std::cout<<"Insert x, y position\n";
  				
  					// the user set x and y position of goal
					std::cin>>posx>>posy;
					std::cout<<"Goal sent\n";
				
					// set the action
					goal.target_pose.pose.position.x=posx;
					goal.target_pose.pose.position.y=posy;
				
					// send the action
					ac.sendGoal(goal);
					set_goal++;
				}
				sleep(1);
				break;
			case '2':
				state=ac.getState();
			
				// check if the goal has not reached
				if (set_goal && state.toString().compare("SUCCEEDED")) {
					ac.cancelGoal();
					ROS_INFO("Goal has been cancelled\n");
					set_goal--;
				}
			
				// if no goal has been set, you cannot delete nothing
				else 
					std::cout<<"All goals have been cancelled: set new one, or exit\n";
				sleep(1);
				break;
		
    			case '3':
    			
    				// the client sends a request to result service
    				client=n.serviceClient<following_goal::NumberGoals>("/result");
    			
    				// call the function for the number of goals
    				number_goals();
    			sleep(1);
    			break;
    			
    		case '4':
    			// before exiting, checks if no goal has been set
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
