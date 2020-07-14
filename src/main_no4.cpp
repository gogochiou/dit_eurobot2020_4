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
#include "main_loop/set_strategy.h"

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
		void status_sub_callback(const std_msgs::Int32::ConstPtr& msg);
        void strategy_sub_callback(const std_msgs::Int32::ConstPtr& msg);
        void change_status(int z); 
        int now_status();
		//void lidarmsg_sub_callback(const lidar_2020::alert_range::ConstPtr& msg);
		//void camera_sub_callback(const main_loop::position::ConstPtr& msg);
		//void publish_(float time);
		//void status_publish();   
		//int exact_status = 0 ;    
		sub_class(){};
		//sub_class(int my_pos_x_ = 0, int my_pos_y_ = 0, int ini_status = 0); //constructor
		
		ros::NodeHandle n;
		//ros::Publisher agent_pub = n.advertise<main_loop::agent>("agent_msg", 1);
		ros::Subscriber ST1_sub = n.subscribe<std_msgs::Int32MultiArray>("rxST1", 1, &sub_class::ST1_sub_callback,this);
		ros::Subscriber ST2_sub = n.subscribe<std_msgs::Int32MultiArray>("rxST2", 1, &sub_class::ST2_sub_callback,this);
		ros::Subscriber status_sub = n.subscribe<std_msgs::Int32>("update_status", 1, &sub_class::status_sub_callback,this);
        ros::Subscriber strategy_sub = n.subscribe<std_msgs::Int32>("strategy", 1, &sub_class::strategy_sub_callback,this);
		//ros::Subscriber camera_sub= n.subscribe<main_loop::position>("enemy_pose", 1, &sub_class::camera_sub_callback,this);
		//ros::Subscriber lidarmsg_sub= n.subscribe<lidar_2020::alert_range>("ranging_alert", 1, &sub_class::lidarmsg_sub_callback,this);
		
		ros::Publisher pub_st1 = n.advertise<std_msgs::Int32MultiArray>("txST1", 1);
		ros::Publisher pub_st2 = n.advertise<std_msgs::Int32MultiArray>("txST2", 1);
		ros::Publisher status_pub = n.advertise<std_msgs::Int32>("pub_status",1);
		//ros::ServiceClient client_path = n.serviceClient<main_loop::path>("path_plan");
		//ros::ServiceClient client_goap = n.serviceClient<main_loop::goap_>("goap_test_v1");

		~sub_class(){}; //destructor
	private:
		int status_dominate = 0 ;
        int sub_GUI_status = 0 ;
        int exact_status = 0 ;
        //int degree_temp = 0 ;
        std_msgs::Int32 status;
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
enum class Status {SET_STRATEGY, RESET, SET_INITIAL_POS, STARTING_SCRIPT, READY, RUN, STOP, IDLE};

int movement_from_goap[2];

int rx0 =0;
int rx1 =0;

int r0 =0;
int r1 =0;
int r2 =0;
int r3 =0;

int my_pos_x=970;
int my_pos_y=2854;
/*int my_pos_x=400;
int my_pos_y=400;*/
int my_degree=100000;

int switch_mode_distance = 40000;//square
int distance_square = 0;

int mode0 = 0;
int mode1 = 0;
int execute_status = 0;

int count1 = 0; // let goap not delete too much mission
std_msgs::Int32 status_fromgui; //choose robot status
int now_strategy = 0;

int cupcolor[5] = {0,0,0,1,0};
int set_goap = 0;

int fordelay = 0;
int count_time = 0;


void sub_class::status_sub_callback(const std_msgs::Int32::ConstPtr& msg){
	sub_GUI_status = msg->data;
    if(status_dominate==0){
        if(sub_GUI_status<4){
            change_status(sub_GUI_status);
        }
        else if(sub_GUI_status==4){
            status_dominate=1;
            change_status(sub_GUI_status);
        }
    }
    status_fromgui.data = now_status();
    ROS_INFO("%d",status_fromgui.data);
}
void sub_class::change_status(int z){
    exact_status = z ; 
}
int sub_class::now_status(){
    return exact_status; 
}

void sub_class::strategy_sub_callback(const std_msgs::Int32::ConstPtr& msg){
    now_strategy = msg->data ; 
}

void sub_class::ST1_sub_callback(const std_msgs::Int32MultiArray::ConstPtr& msg){
	my_pos_x = msg->data[0] ;
	my_pos_y = msg->data[1] ;
	my_degree = msg->data[2] ;
	/*ROS_INFO("%d",my_degree);*/
}

void sub_class::ST2_sub_callback(const std_msgs::Int32MultiArray::ConstPtr& msg){
	mode0 = msg->data[0] ;
	mode1 = msg->data[1] ; 
	execute_status = msg->data[2] ;
	//ROS_INFO("get data2");
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
	ros::Time begin_time ;
    ros::Time now_time ;
	sub_class sub;
	ros::NodeHandle nh;
	ros::ServiceClient client_path = nh.serviceClient<main_loop::path>("path_plan");
	ros::ServiceClient client_goap = nh.serviceClient<main_loop::goap_>("goap_test_v1");
	ros::ServiceClient client_set_goap = nh.serviceClient<main_loop::set_strategy>("set");
	
	

	//初始值
	int margin = 20;
    int angle_margin = 20;

	bool action_done = false;

	RobotState robot;

	//在main裡定義的topic和service所需的傳輸格式需由此先宣告一次
	main_loop::path path_srv;
	main_loop::goap_ goap_srv;
	main_loop::set_strategy strategy_srv;
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
		/*fordelay++;*/
		Status stat;
		if(status_fromgui.data == 4 && my_degree < 1000/*fordelay > 500000*/){
			stat  = static_cast<Status>(5);
			status_fromgui.data = 5;
		}
		else{
			stat  = static_cast<Status>(status_fromgui.data);
		}

		switch(stat){
			case Status::SET_STRATEGY: //0
				r0 = 0x6000;
                r1 = 0;
                r2 = 0;
                r3 = 0;
                break;
            case Status::RESET: //1
                r0 = 0x6000;
                r1 = 0;
                r2 = 0;
                r3 = 0;
                break;

            case Status::SET_INITIAL_POS:   //2
                /*r0 = 0x1000;
                r1 = my_pos_x;
                r2 = my_pos_y;
                r3 = 180;*/
                r0 = 0x5000;
                r1 = 0;
                r2 = 0;
                r3 = 0;
                break;            
            case Status::STARTING_SCRIPT:   //3
                /*r0 = 0x2000;
                r1 = 0;
                r2 = 0;
                r3 = 0;*/
                //fordelay = 0;
                r0 = 0x5000;
                r1 = 0;
                r2 = 0;
                r3 = 0;
                break;

            case Status::READY:{    //4
				//ROS_INFO("hi");
                r0 = 0x5000;
                r1 = 0;
                r2 = 0;
                r3 = 0;
                
                strategy_srv.request.strategy = 1;
                strategy_srv.request.set_finish = 1 ;
                strategy_srv.request.init_pos.push_back(400);
                strategy_srv.request.init_pos.push_back(400);
                strategy_srv.request.cup_color = {}; 
                strategy_srv.request.cup_color.push_back(cupcolor[0]); 
                strategy_srv.request.cup_color.push_back(cupcolor[1]);
                strategy_srv.request.cup_color.push_back(cupcolor[2]); 
                strategy_srv.request.cup_color.push_back(cupcolor[3]);
                strategy_srv.request.cup_color.push_back(cupcolor[4]); 
                if(cupcolor[0]!=2 && set_goap==0){
                    if(client_set_goap.call(strategy_srv)){
                        if(strategy_srv.response.goap_return==true){
                            set_goap=1;
                            ROS_INFO("set goap finish");
                        }
                    }
                    else{
                        ROS_INFO("Failed to call set_strategy");
                    }
                } 
                break;
            }
            
			case Status::RUN:{ //5
				if(count_time == 0){
					begin_time =ros::Time::now();
					count_time =1;
				}
				now_time =ros::Time::now();
				ROS_INFO("%f", (now_time - begin_time).toSec());
				
				/*path*/
				path_srv.request.my_pos_x = my_pos_x;
				path_srv.request.my_pos_y = my_pos_y;
				sub.n.getParam("main_no4/enemy1_x", path_srv.request.enemy1_x);
				sub.n.getParam("main_no4/enemy1_y", path_srv.request.enemy1_y);
				sub.n.getParam("main_no4/enemy2_x", path_srv.request.enemy2_x);
				sub.n.getParam("main_no4/enemy2_y", path_srv.request.enemy2_y);
				sub.n.getParam("main_no4/ally_x", path_srv.request.ally_x);
				sub.n.getParam("main_no4/ally_y", path_srv.request.ally_y);

				/*mission,goap*/
				count1 ++ ;
				/*if(action_done == true && count1<100){
					action_done = false;
				}*/
				
				goap_srv.request.pos={};
				goap_srv.request.time = (now_time - begin_time).toSec();
				goap_srv.request.cup_color.push_back(cupcolor[0]); // 1 - red , 0 - green
				goap_srv.request.cup_color.push_back(cupcolor[1]); 
				goap_srv.request.cup_color.push_back(cupcolor[2]);
				goap_srv.request.cup_color.push_back(cupcolor[3]); 
				goap_srv.request.cup_color.push_back(cupcolor[4]); 
				goap_srv.request.north_or_south = 0 ; 
				goap_srv.request.action_done = action_done;
				goap_srv.request.pos.push_back(my_pos_x);
				goap_srv.request.pos.push_back(my_pos_y); 
				goap_srv.request.my_degree = my_degree;
				if(action_done){
					action_done = false;
				}
				
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
				/*ROS_INFO("%d",goap_srv.response.pos[0]);
				ROS_INFO("%d",goap_srv.response.pos[1]);
				ROS_INFO("%d",goap_srv.response.ST2[0]);
				ROS_INFO("%d",goap_srv.response.ST2[1]); */
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
					/*if(goap_srv.response.mission_name == "lighthouse_walk"){
						robot = RobotState::ON_THE_WAY;
					}
					else{*/
					robot = RobotState::AT_POS;
					ROS_INFO("at pos : true");
					//}
				}
				else{
					robot = RobotState::ON_THE_WAY;
				}

				switch (robot){
					case RobotState::AT_POS:{
						ROS_INFO("hahaahhahahahahahahahahahahahhahahahahhahahahhaahahahhahahahaha");
						r0 = 0x5000;
						r1 = 0;
						r2 = 0;
						r3 = 0;
						if(desire_movement[0] != -1){
							rx0 = desire_movement[0];
							ROS_INFO("mission1 : %d",desire_movement[0]);
						}
						if(desire_movement[1] != -1){
							rx1 = desire_movement[1];
							ROS_INFO("mission1 : %d",desire_movement[1]);
						}
						if(execute_status == 1 && count1 >100 && at_pos(my_pos_x, my_pos_y, my_degree,  desire_pos_x, desire_pos_y, desire_angle, margin, angle_margin)){
							action_done = true;
							count1 = 0;
						}
						break;
					}
					case RobotState::ON_THE_WAY:{
						if( (goap_srv.response.mission_name != "weathervane_walk1") && (goap_srv.response.mission_name != "weathervane_walk2") && (goap_srv.response.mission_name != "weathervane_walk3")&& (goap_srv.response.mission_name != "weathervane_walk4") && (goap_srv.response.mission_name != "weathervane_walk5") && (goap_srv.response.mission_name != "lighthouse_walk") && (goap_srv.response.mission_name != "lighthouse_walk_back") ){
							rx0 = 0;
							rx1 = 0;
							//ROS_INFO("GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG");
						}
						if(client_path.call(path_srv)){
							ROS_INFO("%d", path_srv.request.goal_pos_x);
							ROS_INFO("%d", path_srv.request.goal_pos_y);
							ROS_INFO("path gived degree : %lf", path_srv.response.degree); //float
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
							ROS_INFO("%d", r1);
							ROS_INFO("%d", r2);
							/*for_st1.data.push_back(0x4000);                         //pos mode
							for_st1.data.push_back(path_srv.request.goal_pos_x);
							for_st1.data.push_back(path_srv.request.goal_pos_y);
							for_st1.data.push_back(90);                             // degree*/
						}
						else{
							ROS_INFO("SPEED_MODE");
							r0 = 0x3000;
							r1 = desire_speed;
							r2 = path_srv.response.degree;
							r3 = 0;
							ROS_INFO("%d", r1);
							ROS_INFO("%d", r2);
							/*for_st1.data.push_back(0x3000);                         //speed mode
							for_st1.data.push_back(200);                            //speed
							for_st1.data.push_back(path_srv.response.degree);       //degree
							for_st1.data.push_back(0);                              //0-底盤自選模式*/
						}
						break;
					}	
				}
			}
			/*case Status::STOP:{ //6
                r0 = 0x5000;
                r1 = 0;
                r2 = 0;
                r3 = 0;
                ROS_INFO("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
                break;
			}*/
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
        for_st2.data.push_back(0);
        for_st2.data.push_back(0);
        
		
		sub.status_pub.publish(status_fromgui);
		sub.pub_st1.publish(for_st1);
		sub.pub_st2.publish(for_st2); 
		ros::spinOnce(); //才會callback
	}
	return 0;
}
