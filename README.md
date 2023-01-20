# Rt1_assignment2

The aim of this last assignment is to become more familiar with **ROS**, by developing three ( or four if you want) nodes of a given 3D **Gazebo** and **Rviz** simulation.
The four nodes consist in:

* A node that allows the user to set a goal position ( x, y coordinates) or cancell it;
* A node that publish the current robot position ( x, y coordinates) and velocity ( x, y coordinates);
* A service node that prints the number of goals reached and cancelled;
* A node that print the euclidian distance from the target, and its average speed;

In addition it is required to create **launch file**, which is used to start the whole program and, pass as parameters, the rate for printing the information of last node.

Installing and running
----------------------
For start the whole program, you have to do some several, but fundamental, step. First of all it is important to have **ROS noetic** version on your pc; the best simple suggestion is to have the [**Docker**](https://docs.docker.com/get-docker/) and then follow this [guide](http://wiki.ros.org/ROS/Installation). Then you have to clone two different repository:

* The first one is for the [**Gazebo** action server](https://github.com/CarmineD8/assignment_2_2022), that moves the robot in the simulation, thanks to bug0 algorithm; 
* The second one is mine, that allows you to do what is already explained in the description of the assignment;

You can do it by clicking on the terminal ```git clone link```, where link is the link of the repository. Once you have done this for both repository, you have to put the two pakages in your wokspace; this [link](http://wiki.ros.org/catkin/Tutorials/create_a_workspace) tells you how you can do that; after you have to put the two pakages in your src workspace folder, and then use the command ```catkin_make``` for building everything.
If everything works properly, you can launch the simulation; you must launch firstly the action server, by writing on the terminal ```roslaunch assignment_2_2002 assignment1.launch```, and you can see the spawing of two windows: one is **Gazebo** window, the other one is **Rviz**.
Now you can launch my program, by doing a similar thing: click on terminal ```roslaunch following_goal following_goal.launch```, and it is possible two see four windows; the only one you can interact is the one that tells you to choose one of four option on the menu.
**Important remark**: for launching my launch file, it is important to install **xterm** terminal; you can do thansk to the command ```apt-get install xterm``` on docker terminal. 

## Pseudocode

For achieving what i have already explained before, I built four dirrent nodes. Here I show the pseudocode for the first two: the node for setting the goal and publishing the current position.



