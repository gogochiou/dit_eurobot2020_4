#!/usr/bin/env python
#coding=utf-8
from main_loop.srv import *
import rospy
from goap_setting_no4 import *


def output_processor(output_action):
	output = [-1]*2
	if output_action.type_number is 0:  # 收納,風標,燈塔
		output[0] = 0
		output[1] = output_action.effects[0]
	elif output_action.type_number is 1:  # 放置
		output[0] = 1
		output[1] = output_action.effects[0]
	return output


class mymain:  #main輸入與輸出參數需在此class定義
	action_done = False          # <---
	my_pos = (700, 300)              # <---
	cup_color = [0, 0, 0, 0, 0]  # <---
	north_or_south = 0          # <---
	time = 0                     # <---
	name = 0                     # <---
	#child_name = 0               # <---
	output_speed = 0             # --->
	output_mode = 0            # --->
	output_degree = -1           # --->
	output_position = (300, 300)     # --->
	output = [-1]*2				 # --->
	output_wait = True           # --->
	output_name = "start"        # --->
	#output_child_name = "start"  # --->

go_home_time = 4000
path_done = False
north = 1
south = 2
north_position = (900, 300)
south_position = (1800, 300)
count = 0;
#demo_path, go_home_path = setting(1, mymain.cup_color)


give_next_action = True
go_home_flag = False
#demo_path = []


				   
def handle_return_to_main(req):  #main輸入參數與獲得結果存取處(service回調函式)
	global count
	mymain.action_done = req.action_done  # <----
	mymain.my_pos = (req.pos[0],req.pos[1])
	mymain.cup_color = [req.cup_color[0],req.cup_color[1],req.cup_color[2],req.cup_color[3],req.cup_color[4]]
	rospy.loginfo(mymain.cup_color)
	rospy.loginfo('hi')
	mymain.time = req.time 
	mymain.my_degree = req.my_degree 
	mymain.north_or_south = req.north_or_south 
	mymain.name = req.mission_name
	#mymain.child_name = req.mission_child_name
	count = 2
	return [mymain.output_speed, mymain.output_mode, mymain.output_degree, mymain.output_position, mymain.output, mymain.output_wait, mymain.output_name ] #mymain.output_child_name]

def goap_server():
	
	#定義跨函式但無須傳出的參數:
	global demo_path
	global give_next_action
	global count
	
	#定義goap service name:
	rospy.init_node('goap_main_no4')
	rospy.Service('goap_test_v1', goap_, handle_return_to_main)
	
	
	
	#goap的loop放這:
	while count != 2:
		print("lala")
		
	demo_path, go_home_path = setting(1, mymain.cup_color)
		 
	while True:
		path_done = False
		if mymain.time >= go_home_time and go_home_flag == False:
			demo_path[:] = []
			for action in go_home_path:
				if mymain.north_or_south == north:
					action.position = north_position
				elif mymain.north_or_south == south:
					action.position = south_position
			demo_path = go_home_path
			go_home_flag = True

		path = demo_path[0]
		#mymain.output_child_name = path.name 
		mymain.output_name = path.name
		mymain.output = output_processor(path)
		mymain.output_degree = path.degree
		mymain.output_speed = path.speed
		mymain.output_mode = path.mode
		mymain.output_position = path.position
		mymain.output_wait = path.wait
		#print(str(len(demo_path)))

		if mymain.action_done is True and demo_path[0].name == mymain.name :

			#print(path.name)
			#print(path.position)
			#print(path.degree)
			#print(mymain.output)
			#print()
			if len(demo_path) > 1:
				demo_path.remove(demo_path[0])

			
		
if __name__ == "__main__":
	goap_server()
