# Rt1_assignment2

The aim of this last assignment is to become more familiar with **ROS**, by developing three ( or four if you want) nodes of a given 3D **Gazebo** and **Rviz** simulation.
The four nodes consist in:

* A node that allows the user to set a goal position ( x, y coordinates) or cancell it;
* A node that publishes the current robot position ( x, y coordinates) and velocity ( x, y coordinates);
* A service node that prints the number of goals reached and cancelled;
* A node that prints the euclidian distance from the target, and its average speed;

In addition it is required to create **launch file**, which is used to start the whole program and, pass as parameters, the rate for printing the information of last node.

Installing and running
----------------------
For start the whole program, you have to do some several, but fundamental, step. First of all it is important to have **ROS noetic** version on your pc; the best simple suggestion is to have the [**Docker**](https://docs.docker.com/get-docker/) and then follow this [**ROS guide**](http://wiki.ros.org/ROS/Installation). Then you have to clone two different repository:

* The first one is for the [**Gazebo** action server](https://github.com/CarmineD8/assignment_2_2022), that moves the robot in the simulation, thanks to bug0 algorithm; 
* The second one is mine, that allows you to do what is already explained in the description of the assignment;

You can do it by clicking on the terminal ```git clone link```, where link is the link of the repository. Once you have done this for both repository, you have to put the two pakages in your wokspace; the [workspace guide](http://wiki.ros.org/catkin/Tutorials/create_a_workspace) tells you how you can do that; then you have to put the two pakages, **assignment_2_2022** and **following_goal**, in your src workspace folder, and then use the command ```catkin_make``` for building everything.\
If everything works properly, you can launch the simulation; you must launch firstly the action server, by writing on the terminal ```roslaunch assignment_2_2002 assignment1.launch```, and you can see the spawing of two windows: one is **Gazebo** window, the other one is **Rviz**.

Now you can launch my program, by doing a similar thing: click on terminal ```roslaunch following_goal following_goal.launch```, and it is possible two see four windows; the only one you can interact is the one that tells you to choose one of four option on the menu.

**Important remark**: for launching my launch file, it is important to install **xterm** terminal; you can do thansk to the command ```apt-get install xterm``` on docker terminal. 

## Pseudocode

For achieving what i have already explained before, I built four dirrent nodes. Here I show the pseudocode for the first two: the node for setting the goal and publishing the current position.

* **Set goal node**
```
Function input:
  Pass as argument answer
  No return values
  Print ""Options: 1) Set goal; 2) Cancell current goal; 3) Number of cancelled or reached goal; 4) Exit; Choose one of it"
  Insert var answer 
  RETURN var answer
  
Function number_goals:
  No arguments
  No return values
  Wait for the service;
  Call the service
  SET var reach to the reached value of custome service
  SET var eliminate to the cancelled value of custome service
  Print the var reach and eliminate
  
Function main:
  Call ros init function, with arguments argc, argv and "set_goal"
  Define a NodeHandle
  Create the action client, by passing "/reaching_goal" and true
  Define var answer
  SET var set_goal to 0
  Define var goal
  Print "Waiting for action server to start"
  Call waitForServer function
  WHILE true:
    Call input function, which has the argument answer
    Assign asnwer to the return value of input
    SET var state to the function getState
    IF set_goal is not 0 and the comparison between "SUCCEEDED" and var state returns 0:
      Assign var set_goal-1 to var set_goal
    ENDIF
    SWITCH of var answer
      CASE when var answer is equal to 1:
        Assign the function getState to state
        IF set_goal is not 0 and the comparison between "SUCCEEDED" and var state does not return 0:
          Print "You have to delete a goal before setting a new one"
        ENDIF
        IF set_goal is  to 0 and the comparison between "SUCCEEDED" and var state does not return 0:
          Assign var set_goal-1 to var set_goal
        END IF
        ELSE IF var set_goal is 0
          Print "Insert x, y position\n
          Insert var posx and posy
          Assign var posx to the var goal.target_pose.pose.position.x
          Assign var posy to the var goal.target_pose.pose.position.y
          Call the function sendGoal with argument the var goal
       ENDELSEIF
       Call the sleep function
       TERMINATE
       CASE when var answer is equal to 2:
        Assign the return value of function getState to var state
          IF the comparison between "SUCCEEDED" and var state does not return 0
            Call the function cancellGoal
            Print "Goal has been cancelled"
       
```

