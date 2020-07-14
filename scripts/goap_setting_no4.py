#! python
#coding=utf-8
"""
Set all actions, goals and current world states here!!
this .py file is the only place you will need to change
=======================================================
"""
from action import *

#color = [1,1,1,1,1]  # 1 - green  0 - red
color = []
outside_cup_pos = [(117,2004),(117,2069),(117,2137),(117,2208),(117,2315)] #need camera
#0,1,0,1,0
inside_cup_pos = [(1728,2855),(1576,2855),(1422,2855),(1496,2855),(1638,2855)]  #knowned
redcount = 0
greencount = 0
run_point_count = 0
put_cup_count = 0
demo_path =[]
speed1 = 300
	
'''class Action:
	def __init__(self, name, preconditions, effects, child_action, position, degree, speed, cost, mode, speed_mode_radius, type_number, cup_number, wait):
		self.name = name
		self.preconditions = preconditions
		self.effects = effects
		self.child_action = child_action
		self.cost = cost
		self.position = position
		self.wait = wait
		self.degree = degree
		self.type_number = type_number
		self.mode = mode  # speed mode or position mode
		self.speed_mode_radius = speed_mode_radius
		self.speed = speed
		self.cup_number = cup_number
		self.tangent_position = []
		self.tangent_angle = []
		self.path = []
		self.priority = 0
		self.required_world_state = []
		self.result_world_state = []
		print(self.name,self.position, sep='\t')'''

def grab_fuc(): #收納mode
	global greencount,run_point_count
	run_point_count +=1
	if run_point_count <=5:
		if run_point_count <= greencount :
			if run_point_count <= 3 :
				return Action('grab_green_in_mid' + str(run_point_count), [], [2], [], outside_cup_pos[run_point_count-1], 270, 150, 0, 0, 0, 0, 0, True)
			elif 3 < run_point_count <= 5 :
				return Action('grab_green_in_right' + str(run_point_count), [], [1], [], outside_cup_pos[run_point_count-1], 270, 150, 0, 0, 0, 0, 0, True)
		else :
			if run_point_count <= 3 :
				return Action('grab_red_in_mid' + str(run_point_count), [], [2], [], outside_cup_pos[run_point_count-1], 270, 150, 0, 0, 0, 0, 0, True)
			elif 3 < run_point_count <= 5 :
				return Action('grab_red_in_right' + str(run_point_count), [], [1], [], outside_cup_pos[run_point_count-1], 270, 150, 0, 0, 0, 0, 0, True)		
	else:
		if run_point_count <= 8 :
			return Action('grab_green_in_left' + str(run_point_count), [], [3], [], inside_cup_pos[run_point_count-6], 270, 150, 0, 0, 0, 0, 0, True)
		else :
			if run_point_count ==9  :
				return Action('grab_red_in_right' + str(run_point_count), [], [1], [], inside_cup_pos[run_point_count-6], 270, 150, 0, 0, 0, 0, 0, True)
			else:
				return Action('grab_red_in_hold' + str(run_point_count), [], [10], [], inside_cup_pos[run_point_count-6], 270, 150, 0, 0, 0, 0, 0, True)
'''def take_in_fuc():
	global redcount,run_point_count
	if run_point_count <= 3 :
		return Action('take_in_mid', [], [1], outside_cup_pos[run_point_count-1], 3, 270, 150, 1, 15, 5, False, True)
	elif 3 < run_point_count <= 5 :
		return Action('take_in_right', [], [1], outside_cup_pos[run_point_count-1], 3, 270, 150, 2, 15, 5, False, True)
	elif 8 >= run_point_count > 5 :
		return Action('take_in_left', [], [1], inside_cup_pos[run_point_count-5-1], 3, 270, 150, 3, 15, 5, False, True)
	elif run_point_count ==9  :
		return Action('take_in_right', [], [1], inside_cup_pos[run_point_count-5-1], 3, 270, 150, 2, 15, 5, False, True)
	else:
		return Action('take_in_hold', [], [1], inside_cup_pos[run_point_count-5-1], 3, 270, 150, 4, 15, 5, False, True)'''

def put_fuc(): #放置
	global redcount,put_cup_count
	put_cup_count += 1
	if put_cup_count == 1 :
		return Action('red_on-hand_put', [], [10], [], (300,200), 270, 150, 0, 0, 0, 1, 0, True)
	
	else:
		if put_cup_count < 5 :
			cup_number = 1+(put_cup_count-2)*3
		elif 5 <= put_cup_count < 8 :
			cup_number = 2+(put_cup_count-5)*3
		else :
			cup_number = 3+(put_cup_count-8)*3
		if redcount == 0 :
			if put_cup_count <= 2 :
				return Action('red_right_put' + str(put_cup_count), [], [cup_number], [], (300+100*(put_cup_count-1),200), 270, 150, 0, 0, 0, 1, 0, True)
			elif 2 < put_cup_count <= 4 :
				return Action('green_right_put' + str(put_cup_count), [], [cup_number], [], (300+100*(put_cup_count-3),500), 270, 150, 0, 0, 0, 1, 0, True)
			elif 4 < put_cup_count <= 7 :
				return Action('green_mid_put' + str(put_cup_count), [], [cup_number], [], (300+100*(put_cup_count-3),500), 270, 150, 0, 0, 0, 1, 0, True)
			else:
				return Action('green_left_put' + str(put_cup_count), [], [cup_number], [], (300+100*(put_cup_count-3),500), 270, 150, 0, 0, 0, 1, 0, True)
		elif redcount == 1 :
			if put_cup_count <= 3 :
				return Action('red_right_put' + str(put_cup_count), [], [cup_number], [], (300+100*(put_cup_count-1),200), 270, 150, 0, 0, 0, 1, 0, True)
			elif 3 < put_cup_count <= 4 :
				return Action('green_right_put' + str(put_cup_count), [], [cup_number], [], (300+100*(put_cup_count-4),500), 270, 150, 0, 0, 0, 1, 0, True)
			elif 4 < put_cup_count <= 7 :
				return Action('green_mid_put' + str(put_cup_count), [], [cup_number], [], (300+100*(put_cup_count-4),500), 270, 150, 0, 0, 0, 1, 0, True)
			else:
				return Action('green_left_put' + str(put_cup_count), [], [cup_number], [], (300+100*(put_cup_count-4),500), 270, 150, 0, 0, 0, 1, 0, True)
		elif redcount == 2 :
			if put_cup_count <= 4 :
				return Action('red_right_put' + str(put_cup_count), [], [cup_number], [], (300+100*(put_cup_count-1),200), 270, 150, 0, 0, 0, 1, 0, True)
			elif 4 < put_cup_count <= 7 :
				return Action('green_mid_put' + str(put_cup_count), [], [cup_number], [], (300+100*(put_cup_count-5),500), 270, 150, 0, 0, 0, 1, 0, True)
			else:
				return Action('green_left_put' + str(put_cup_count), [], [cup_number], [], (300+100*(put_cup_count-5),500), 270, 150, 0, 0, 0, 1, 0, True)
		elif redcount == 3 :
			if put_cup_count <= 4 :
				return Action('red_right_put' + str(put_cup_count), [], [cup_number], [], (300+100*(put_cup_count-1),200), 270, 150, 0, 0, 0, 1, 0, True)
			elif 4 < put_cup_count <= 5 :
				return Action('red_mid_put' + str(put_cup_count), [], [cup_number], [], (300+100*(put_cup_count-1),200), 270, 150, 0, 0, 0, 1, 0, True)
			elif 5 < put_cup_count <= 7 :
				return Action('green_mid_put' + str(put_cup_count), [], [cup_number], [], (300+100*(put_cup_count-6),500), 270, 150, 0, 0, 0, 1, 0, True)
			else:
				return Action('green_left_put' + str(put_cup_count), [], [cup_number], [], (300+100*(put_cup_count-6),500), 270, 150, 0, 0, 0, 1, 0, True)
		elif redcount == 4 :
			if put_cup_count <= 4 :
				return Action('red_right_put' + str(put_cup_count), [], [cup_number], [], (300+100*(put_cup_count-1),200), 270, 150, 0, 0, 0, 1, 0, True)
			elif 4 < put_cup_count <= 6 :
				return Action('red_mid_put' + str(put_cup_count), [], [cup_number], [], (300+100*(put_cup_count-1),200), 270, 150, 0, 0, 0, 1, 0, True)
			elif 6 < put_cup_count <= 7 :
				return Action('green_mid_put' + str(put_cup_count), [], [cup_number], [], (300+100*(put_cup_count-7),500), 270, 150, 0, 0, 0, 1, 0, True)
			else:
				return Action('green_left_put' + str(put_cup_count), [], [cup_number], [], (300+100*(put_cup_count-7),500), 270, 150, 0, 0, 0, 1, 0, True)
		elif redcount == 5 :
			if put_cup_count <= 4 :
				return Action('red_right_put' + str(put_cup_count), [], [cup_number], [], (300+100*(put_cup_count-1),200), 270, 150, 0, 0, 0, 1, 0, True)
			elif 4 < put_cup_count <= 7 :
				return Action('red_mid_put' + str(put_cup_count), [], [cup_number], [], (300+100*(put_cup_count-1),200), 270, 150, 0, 0, 0, 1, 0, True)
			else:
				return Action('green_left_put' + str(put_cup_count), [], [cup_number], [], (300+100*(put_cup_count-8),500), 270, 150, 0, 0, 0, 1, 0, True)
'''def take_out_fuc():
	global greencount,put_cup_count
	if greencount == 0 :
		if put_cup_count <= 2 :
			c11 = Action('put_cup', [], [1], (300+100*(put_cup_count-1),200), 3, 270, 150, 4, 15, 5, False, True)
		else:
			c11 = Action('put_cup', [], [1], (300+100*(put_cup_count-3),500), 3, 270, 150, 4, 15, 5, False, True)
	elif greencount == 1 :
		if put_cup_count <= 3 :
			c11 = Action('put_cup', [], [1], (300+100*(put_cup_count-1),200), 3, 270, 150, 4, 15, 5, False, True)
		else:
			c11 = Action('put_cup', [], [1], (300+100*(put_cup_count-4),500), 3, 270, 150, 4, 15, 5, False, True)
	elif greencount == 2 :
		if put_cup_count <= 4 :
			c11 = Action('put_cup', [], [1], (300+100*(put_cup_count-1),200), 3, 270, 150, 4, 15, 5, False, True)
		else:
			c11 = Action('put_cup', [], [1], (300+100*(put_cup_count-5),500), 3, 270, 150, 4, 15, 5, False, True)
	elif greencount == 3 :
		if put_cup_count <= 5 :
			c11 = Action('put_cup', [], [1], (300+100*(put_cup_count-1),200), 3, 270, 150, 4, 15, 5, False, True)
		else:
			c11 = Action('put_cup', [], [1], (300+100*(put_cup_count-6),500), 3, 270, 150, 4, 15, 5, False, True)
	elif greencount == 4 :
		if put_cup_count <= 6 :
			c11 = Action('put_cup', [], [1], (300+100*(put_cup_count-1),200), 3, 270, 150, 4, 15, 5, False, True)
		else:
			c11 = Action('put_cup', [], [1], (300+100*(put_cup_count-7),500), 3, 270, 150, 4, 15, 5, False, True)
	elif greencount == 5 :
		if put_cup_count <= 7 :
			c11 = Action('put_cup', [], [1], (300+100*(put_cup_count-1),200), 3, 270, 150, 4, 15, 5, False, True)
		else:
			c11 = Action('put_cup', [], [1], (300+100*(put_cup_count-8),500), 3, 270, 150, 4, 15, 5, False, True)
	put_cup_count += 1
	return c11'''

# name, preconditions, effects, position, cost, degree, speed, mode, speed_mode_radius, number, iscup, iswait
# 0-收納,風標,燈塔 ,1-放置 ,2- ,3- ,4- ,0-0 reset
def setting(mode,cup_color):
	global redcount,greencount,color,speed1
	demo_path =[]
	color = cup_color
	for i in range(5) :
		if color[i]==1 :
			 greencount+=1
	redcount = 5-greencount
	for i in range(5) :
		if color[i]==1 :
			for k in range(i) :
				if(color[i-k-1] != 1):
					color[i-k],color[i-k-1] = color[i-k-1],color[i-k]				
					outside_cup_pos[i-k],outside_cup_pos[i-k-1] = outside_cup_pos[i-k-1],outside_cup_pos[i-k]
	#find_point_order()
	#print(outside_cup_pos)
	#print(color)
	
	if mode == 1:
		#back to home
		c1 = Action('go_home',[], [0], [], (1650, 865), 270, 200, 0, 0, 0, 0, 0, True)

		#grab_cup
		#100 = Action('grab_red_in_hold1' , [], [1], [], (1450,2845),180, 200, 0, 0, 0, 0, 0, True)
		c15 = Action('goto_outside_point',[], [], [], (117, 1973), 270, 150, 0, 0, 0, 0, 0, True)
		c16 = Action('goto_inside_point1',[], [], [], (175, 2714), 270, 150, 0, 0, 0, 0, 0, True)
		c17 = Action('goto_inside_point2',[], [], [], (175, 2714), 185, 150, 0, 0, 0, 0, 0, True)
	
		#lighthouse
		c4 = Action('lighthouse_push',[], [4], [], (190, 2756), 180, 200, 0, 0, 0, 0, 0, True)
		c5 = Action('lighthouse_walk',[], [4], [], (150, 2756), 180, 200, 0, 0, 0, 0, 0, True)
		c6 = Action('lighthouse_walk_back',[], [0], [], (230,2756), 180, 200, 0, 0, 0, 0, 0, True)
		'''c4 = Action('lighthouse_push1',[], [4], [], (400, 800), 90, 150, 0, 0, 0, 0, 0, True)
		c5 = Action('lighthouse_walk',[], [4], [], (400, 1000), 90, 150, 0, 0, 0, 0, 0, True)
		c32 = Action('lighthouse_walk_back',[], [0], [], (400, 800), 90, 150, 0, 0, 0, 0, 0, True)'''
				

		#Weathervane
		c7 = Action('grab_red_in_hold1' , [], [1], [], (1450,2854),180, 200, 0, 0, 0, 0, 0, True)
		c8787 = Action('walkout1',[], [0], [], (1350, 2845), 164,200, 0, 0, 0, 0, 0, True)
		#c7 = Action('walkout1',[], [0], [], (1350, 2845), 164,200, 0, 0, 0, 0, 0, True)
		c8 = Action('walkout2',[], [0], [], (1815,2600), 90, 200, 0, 0, 0, 0, 0, True) #1835,2600
		c20 = Action('walkout3',[], [0], [], (1815, 2240), 90, 200, 0, 0, 0, 0, 0, True)
		#2900
		#c9 = Action('walkout3',[], [0], [], (1835, 2880), 90, 150, 0, 0, 0, 0, 0, True)
		c9 = Action('weathervane_push',[], [11], [], (1815,2870), 90, 200, 0, 0, 0, 0, 0, True)
		c87 = Action('weathervane_walk1',[], [11], [], (1815,2870), 90, 350, 0, 0, 0, 0, 0, True)
		c10 = Action('weathervane_walk2',[], [11], [], (1815, 2360), 90, 350, 0, 0, 0, 0, 0, True)
		caa = Action('weathervane_walk3',[], [11], [], (1770, 2290), 70, 200, 0, 0, 0, 0, 0, True)
		cbb = Action('weathervane_walk4',[], [11], [], (1700, 2220), 40, 200, 0, 0, 0, 0, 0, True)
		c11 = Action('weathervane_walk5',[], [0], [], (1700, 2300), 90, 200, 0, 0, 0, 0, 0, True)
		#c11 = Action('turn_body',[], [0], [], (1600, 2280), 110, 600, 0, 0, 0, 0, 0, True)
		#c10 = Action('weathervane_walk',[], [0], [], (1820, 2200), 180, 600, 0, 0, 0, 0, 0, True) #die 
		#c6 = Action('weathervane_push',[], [11], [], (400, 1200), 90, 150, 0, 0, 0, 0, 0, True)
		#c7 = Action('weathervane_walk',[], [0], [], (400, 1600), 90, 150, 0, 0, 0, 0, 0, True)
		#c11 = Action('turn_body1',[], [0], [], (1820, 2200), 180, 300, 0, 0, 0, 0, 0, True)

		#flag
		#c10 =  Action('flag',[], [12], [], (400, 2000), 90, 150, 0, 0, 0, 0, 0, True)
		c13 =  Action('flag',[], [12], [], (110,2756), 180, 200, 0, 0, 0, 0, 0, True)
		
		#take_out_cup
		#c3 = put_fuc()
		
		#path point
		c12 = Action('path_point1',[], [0], [], (1400, 2756), 180, 200, 0, 0, 0, 0, 0, True)
		c14 = Action('path_point2',[], [0], [], (359, 2756), 180, 200, 0, 0, 0, 0, 0, True)

		
		#demo_path = [grab_fuc(),grab_fuc(),grab_fuc(),grab_fuc(),grab_fuc(),grab_fuc(),grab_fuc(),grab_fuc(),grab_fuc(),grab_fuc()]
		#demo_path = [put_fuc(),put_fuc(),put_fuc(),put_fuc(),put_fuc(),put_fuc(),put_fuc(),put_fuc(),put_fuc(),put_fuc()]
		demo_path = [c7,c8787,c8,c9,c87,c10,caa,c11,c12,c4,c5,c6]
		'''c20,c21,c22,c23,c24,c25,c26,c27,c28,c29,c30,'''
		go_home_path = [c13]

		#print(str(len(demo_path)))
	return demo_path, go_home_path

#setting(1,0)
