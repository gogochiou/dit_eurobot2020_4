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


int my_pos_x=400;
int my_pos_y=400;
int my_degree=0;

int switch_mode_distance = 250000;//square
int distance_square = 0;

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

int main(int argc, char **argv){

	ros::init(argc, argv, "main_no4");

	sub_class sub;
	ros::NodeHandle nh;
	ros::ServiceClient client_path = nh.serviceClient<main_loop::path>("path_plan");
    ros::ServiceClient client_goap = n.serviceClient<main_loop::goap_>("goap_test_v1");
    main_loop::path path_srv;
    //main_loop::goap_ goap_srv;

    while(ros::ok()){
        /*mission*/
        

    	/*path*/
    	path_srv.request.my_pos_x = my_pos_x;
        path_srv.request.my_pos_y = my_pos_y;
        sub.n.getParam("main_no4/goal_pos_x", path_srv.request.goal_pos_x);
		sub.n.getParam("main_no4/goal_pos_y", path_srv.request.goal_pos_y);
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
        
        if(client_path.call(path_srv)){
            ROS_INFO("%d", path_srv.request.goal_pos_x);
            ROS_INFO("%d", path_srv.request.goal_pos_y);
            ROS_INFO("%lf", path_srv.response.degree); //float
        }
        else{
            ROS_ERROR("Failed to call path_plan");
        }
    
        std_msgs::Int32MultiArray for_st1;
        if( (my_pos_x != path_srv.request.goal_pos_x) || (my_pos_y != path_srv.request.enemy1_y) ){
            distance_square = pow((my_pos_x - path_srv.request.goal_pos_x),2) + pow((my_pos_y - path_srv.request.goal_pos_y),2);
            if(distance_square < switch_mode_distance){
                ROS_INFO("POS_MODE");
                for_st1.data.push_back(0x4000);                         //pos mode
                for_st1.data.push_back(path_srv.request.goal_pos_x);
                for_st1.data.push_back(path_srv.request.goal_pos_y);
                for_st1.data.push_back(90);                             // degree
            }
            else{
                ROS_INFO("SPEEd_MODE");
                for_st1.data.push_back(0x3000);                         //speed mode
                for_st1.data.push_back(200);                            //speed
                for_st1.data.push_back(path_srv.response.degree);       //degree
                for_st1.data.push_back(0);                              //0-底盤自選模式
            }
        }
        else{
            ROS_INFO("reach : %d , %d ",my_pos_x,my_pos_y);
        }

        sub.pub_st1.publish(for_st1); 
        ros::spinOnce(); //才會callback
    }
    return 0;
}