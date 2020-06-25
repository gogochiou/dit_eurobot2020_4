#include "ros/ros.h"
#include "std_msgs/String.h"
#include <std_msgs/Int32MultiArray.h>
#include <std_msgs/Int32.h>
#include "main_loop/path.h"
#include "main_loop/agent.h"
#include "main_loop/goap_.h"
#include <main_loop/from_goap.h>
#include "main_loop/main_state.h"
#include <main_loop/goap_debug.h>
#include "main_loop/main_debug.h"
#include "main_loop/world_state.h"

#include <sstream>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <vector>
#include <cmath>
#include <stdlib.h>

using namespace std;

class sub_class{
	public:
		void ST1_sub_callback(const std_msgs::Int32MultiArray::ConstPtr& msg); //constPtr let the variable 'msg' can not be change in this function
		void ST2_sub_callback(const std_msgs::Int32MultiArray::ConstPtr& msg); 
		//void lidarmsg_sub_callback(const lidar_2020::alert_range::ConstPtr& msg);
		//void camera_sub_callback(const main_loop::position::ConstPtr& msg);
		//void status_sub_callback(const std_msgs::Int32::ConstPtr& msg);
		//void publish_(float time);
		//void status_publish();   
		//int exact_status = 0 ;    
		sub_class(){};
		//sub_class(int my_pos_x_ = 0, int my_pos_y_ = 0, int ini_status = 0); //constructor
		
		ros::NodeHandle n;
		//ros::Publisher status_pub = n.advertise<std_msgs::Int32>("pub_status",1); 
		//ros::Publisher agent_pub = n.advertise<main_loop::agent>("agent_msg", 1);
		//ros::Subscriber status_sub = n.subscribe<std_msgs::Int32>("update_status", 1, &sub_class::status_sub_callback,this);
		ros::Subscriber ST1_sub = n.subscribe<std_msgs::Int32MultiArray>("rxST1", 1, &sub_class::ST1_sub_callback,this);
		ros::Subscriber ST2_sub = n.subscribe<std_msgs::Int32MultiArray>("rxST2", 1, &sub_class::ST2_sub_callback,this);
		//ros::Subscriber camera_sub= n.subscribe<main_loop::position>("enemy_pose", 1, &sub_class::camera_sub_callback,this);
		//ros::Subscriber lidarmsg_sub= n.subscribe<lidar_2020::alert_range>("ranging_alert", 1, &sub_class::lidarmsg_sub_callback,this);
		
		ros::Publisher pub_st1 = n.advertise<std_msgs::Int32MultiArray>("txST1", 1);
		ros::Publisher pub_st2 = n.advertise<std_msgs::Int32MultiArray>("txST2", 1);
		//ros::ServiceClient client_path = n.serviceClient<main_loop::path>("path_plan");
		//ros::ServiceClient client_goap = n.serviceClient<main_loop::goap_>("goap_test_v1");

		~sub_class(){}; //destructor
};

class action{
public:
    action(int x, int y, int movement_num[], int what_angle, int how_fast, bool is_wait, int what_mode){
        goal_pos_x = x;
        goal_pos_y = y;
        for(int i = 0; i < 2; i ++){
            movement[i] = movement_num[i];
        }
        angle = what_angle;
        speed = how_fast;
        wait = is_wait;
        mode = what_mode;
    }
    action(){};
    int PosX(){
        return goal_pos_x;
    }
    int PosY(){
        return goal_pos_y;
    }
    int * Movement(){
        return  movement;
    }
    int Speed(){
        return speed;
    }
    bool Wait(){
        return wait;
    }
    int Angle(){
        return angle;
    }
    int Mode(){
        return mode;
    }
private:
    int goal_pos_x;
    int goal_pos_y;
    int movement[2];
    int speed;
    int angle;
    bool wait;
    int mode;
};

enum class RobotState {AT_POS, ON_THE_WAY, BLOCKED};

int movement_from_goap[2];

int rx0 =0;
int rx1 =0;

int r0 =0;
int r1 =0;
int r2 =0;
int r3 =0;

int my_pos_x=400;
int my_pos_y=400;
int my_degree=0;

int switch_mode_distance = 4000000;//square
int distance_square = 0;

int mode0 = 0;
int mode1 = 0;
int execute_status = 0;

void sub_class::ST1_sub_callback(const std_msgs::Int32MultiArray::ConstPtr& msg){
	my_pos_x = msg->data[0] ;
	my_pos_y = msg->data[1] ;
	my_degree = msg->data[2] ;
	ROS_INFO("get data");
}

void sub_class::ST2_sub_callback(const std_msgs::Int32MultiArray::ConstPtr& msg){
	mode0 = msg->data[0] ;
	mode1 = msg->data[1] ; 
	execute_status = msg->data[2] ;
	//reserved = msg->data[3] ; 
	//CRC = msg->data[4]  
}

bool at_pos(int x, int y, int deg, int c_x, int c_y, int c_deg, int m, int angle_m){
    //計算誤差
    bool at_p = false;
    int exact_angle_difference=0 ;
    int angle_difference_1=abs(c_deg-deg);
    int angle_difference_2=360-abs(c_deg-deg);
    if(angle_difference_1 > angle_difference_2){
        exact_angle_difference = angle_difference_2;
    }else{
        exact_angle_difference = angle_difference_1;
    }    
    if(abs(x - c_x) < m && abs(y - c_y) < m && exact_angle_difference < angle_m){
        at_p = true;
    }
    return at_p;
}

int main(int argc, char **argv){

	ros::init(argc, argv, "main_no4");

	sub_class sub;
	ros::NodeHandle nh;
	ros::ServiceClient client_path = nh.serviceClient<main_loop::path>("path_plan");
	ros::ServiceClient client_goap = nh.serviceClient<main_loop::goap_>("goap_test_v1");

	//初始值
	int margin = 50;
    int angle_margin = 10;

	bool action_done = false;

	RobotState robot;

	//在main裡定義的topic和service所需的傳輸格式需由此先宣告一次
	main_loop::path path_srv;
	main_loop::goap_ goap_srv;
	/*mission,goap*/
	/*goap_srv.request.pos={400,400};
	goap_srv.request.time = 0.0;
	goap_srv.request.cup_color.push_back(0); // 1 - red , 0 - green
	goap_srv.request.cup_color.push_back(1); 
	goap_srv.request.cup_color.push_back(0);
	goap_srv.request.cup_color.push_back(1); 
	goap_srv.request.cup_color.push_back(0); 
	goap_srv.request.north_or_south = 0 ; 
	goap_srv.request.action_done = false;
	goap_srv.request.pos.push_back(400);
	goap_srv.request.pos.push_back(400); 
    goap_srv.request.my_degree = 0;*/
	

	while(ros::ok()){
		
		
		/*path*/
		path_srv.request.my_pos_x = my_pos_x;
		path_srv.request.my_pos_y = my_pos_y;
		//sub.n.getParam("main_no4/goal_pos_x", path_srv.request.goal_pos_x);
		//sub.n.getParam("main_no4/goal_pos_y", path_srv.request.goal_pos_y);
		sub.n.getParam("main_no4/enemy1_x", path_srv.request.enemy1_x);
		sub.n.getParam("main_no4/enemy1_y", path_srv.request.enemy1_y);
		sub.n.getParam("main_no4/enemy2_x", path_srv.request.enemy2_x);
		sub.n.getParam("main_no4/enemy2_y", path_srv.request.enemy2_y);
		sub.n.getParam("main_no4/ally_x", path_srv.request.ally_x);
		sub.n.getParam("main_no4/ally_y", path_srv.request.ally_y);
		// path_srv.request.goal_pos_x = 1000;
		// path_srv.request.goal_pos_y = 1000;
		// path_srv.request.enemy1_x = 1300;
		// path_srv.request.enemy1_y = 1800;
		// path_srv.request.enemy2_x = 5000;
		// path_srv.request.enemy2_y = 5000;
		// path_srv.request.ally_x = 1300;
		// path_srv.request.ally_y = 1300;
		/*ROS_INFO("%d",my_pos_x);
		ROS_INFO("%d",my_pos_y);
		ROS_INFO("%d",path_srv.request.goal_pos_x);		
		ROS_INFO("%d",path_srv.request.goal_pos_y);
		ROS_INFO("%d",path_srv.request.enemy1_x);
		ROS_INFO("%d",path_srv.request.enemy1_y);
		ROS_INFO("%d",path_srv.request.enemy2_x);
		ROS_INFO("%d",path_srv.request.enemy2_y);
		ROS_INFO("%d",path_srv.request.ally_x);
		ROS_INFO("%d",path_srv.request.ally_y);*/


		/*mission,goap*/
		goap_srv.request.pos={};
		goap_srv.request.time = 0.0;
        goap_srv.request.cup_color.push_back(0); // 1 - red , 0 - green
        goap_srv.request.cup_color.push_back(1); 
        goap_srv.request.cup_color.push_back(0);
        goap_srv.request.cup_color.push_back(1); 
        goap_srv.request.cup_color.push_back(0); 
        goap_srv.request.north_or_south = 0 ; 
        goap_srv.request.action_done = false;
        goap_srv.request.pos.push_back(400);
        goap_srv.request.pos.push_back(400); 
        goap_srv.request.my_degree = my_degree;
        goap_srv.request.mission_name = "hi";
        ROS_INFO("%d",goap_srv.request.action_done);
        
        
		if(client_goap.call(goap_srv)){  
            movement_from_goap[0]=goap_srv.response.ST2[0];
            movement_from_goap[1]=goap_srv.response.ST2[1];
             //for path plan
            path_srv.request.goal_pos_x = goap_srv.response.pos[0];
            path_srv.request.goal_pos_y = goap_srv.response.pos[1];
        }
		else{
            ROS_INFO("Failed to call goap_test");
        } 
        
		goap_srv.request.mission_name = goap_srv.response.mission_name ;
        //goap_srv.request.mission_child_name = goap_srv.response.mission_child_name ;

		//將goap所需的資料存入action
		ROS_INFO("%d",goap_srv.response.pos[0]);
		ROS_INFO("%d",goap_srv.response.pos[1]);
		ROS_INFO("%d",goap_srv.response.ST2[0]);
		ROS_INFO("%d",goap_srv.response.ST2[1]);           
        action act(goap_srv.response.pos[0], goap_srv.response.pos[1], movement_from_goap, goap_srv.response.degree, goap_srv.response.speed, goap_srv.response.is_wait, goap_srv.response.mode);
        int desire_pos_x = act.PosX();
        int desire_pos_y = act.PosY();
        int*desire_movement;
        int desire_speed = act.Speed();
        int desire_mode = act.Mode();
    	bool desire_wait = act.Wait();
        int desire_angle = act.Angle();
        desire_movement = act.Movement();

		if (at_pos(my_pos_x, my_pos_y, my_degree,  desire_pos_x, desire_pos_y, desire_angle, margin, angle_margin)){
			robot = RobotState::AT_POS;
		}
		else{
			robot = RobotState::ON_THE_WAY;
		}

		switch (robot){
			case RobotState::AT_POS:{
				if(desire_movement[0] != -1){
					rx0 = desire_movement[0];
				}
				if(desire_movement[1] != -1){
					rx1 = desire_movement[1];
				}
				if(execute_status == 1){
					action_done = true;
				}
				break;
			}
			case RobotState::ON_THE_WAY:{
				if(client_path.call(path_srv)){
					//ROS_INFO("%d", path_srv.request.goal_pos_x);
					//ROS_INFO("%d", path_srv.request.goal_pos_y);
					//ROS_INFO("%lf", path_srv.response.degree); //float
				}
				else{
					ROS_ERROR("Failed to call path_plan");
				}
				
				distance_square = pow((my_pos_x - desire_pos_x),2) + pow((my_pos_y - desire_pos_y),2);
				if(distance_square < switch_mode_distance){
					ROS_INFO("POS_MODE");
					r0 = 0x4000;
					r1 = desire_pos_x;
					r2 = desire_pos_y;
					r3 = desire_angle;
					/*for_st1.data.push_back(0x4000);                         //pos mode
					for_st1.data.push_back(path_srv.request.goal_pos_x);
					for_st1.data.push_back(path_srv.request.goal_pos_y);
					for_st1.data.push_back(90);                             // degree*/
				}
				else{
					ROS_INFO("SPEEd_MODE");
					r0 = 0x3000;
                    r1 = desire_speed;
                    r2 = my_degree;
                    r3 = 0;
					/*for_st1.data.push_back(0x3000);                         //speed mode
					for_st1.data.push_back(200);                            //speed
					for_st1.data.push_back(path_srv.response.degree);       //degree
					for_st1.data.push_back(0);                              //0-底盤自選模式*/
				}
				break;
			}		
		}

		//give ST
        std_msgs::Int32MultiArray for_st1;
        for_st1.data.push_back(r0);
        for_st1.data.push_back(r1);
        for_st1.data.push_back(r2);
        for_st1.data.push_back(r3);

        std_msgs::Int32MultiArray for_st2;
        for_st2.data.push_back(rx0);
        for_st2.data.push_back(rx1);
		

		sub.pub_st1.publish(for_st1);
		sub.pub_st2.publish(for_st2); 
		ros::spinOnce(); //才會callback
	}
	return 0;
}
