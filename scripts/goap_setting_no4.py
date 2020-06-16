#! python
"""
Set all actions, goals and current world states here!!
this .py file is the only place you will need to change
=======================================================
"""
from action import *

#color = [1,1,1,1,1]  # 1 - red  0 - green
color = []
outside_cup_pos = [(200,600),(200,700),(200,800),(200,900),(200,1000)] #need camera
inside_cup_pos = [(600,200),(700,200),(800,200),(900,200),(1000,200)]  #knowned
redcount = 0
greencount = 0
run_point_count = 0
put_cup_count = 0
demo_path =[]
	
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
	global redcount,run_point_count
	run_point_count +=1
	if run_point_count <=5:
		if run_point_count <= redcount :
			if run_point_count <= 3 :
				return Action('grab_reb_in_mid', [], [2], [], outside_cup_pos[run_point_count-1], 270, 150, 0, 0, 0, 0, 0, True)
			elif 3 < run_point_count <= 5 :
				return Action('grab_red_in_right', [], [1], [], outside_cup_pos[run_point_count-1], 270, 150, 0, 0, 0, 0, 0, True)
		else :
			if run_point_count <= 3 :
				return Action('grab_green_in_mid', [], [2], [], outside_cup_pos[run_point_count-1], 270, 150, 0, 0, 0, 0, 0, True)
			elif 3 < run_point_count <= 5 :
				return Action('grab_green_in_right', [], [1], [], outside_cup_pos[run_point_count-1], 270, 150, 0, 0, 0, 0, 0, True)		
	else:
		if run_point_count <= 8 :
			return Action('grab_red_in_left', [], [3], [], inside_cup_pos[run_point_count-6], 270, 150, 0, 0, 0, 0, 0, True)
		else :
			if run_point_count ==9  :
				return Action('grab_green_in_right', [], [1], [], inside_cup_pos[run_point_count-6], 270, 150, 0, 0, 0, 0, 0, True)
			else:
				return Action('grab_green_in_hold', [], [10], [], inside_cup_pos[run_point_count-6], 270, 150, 0, 0, 0, 0, 0, True)
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
	global greencount,put_cup_count
	put_cup_count += 1
	if put_cup_count == 1 :
		return Action('green_on-hand_put', [], [10], [], (300,200), 270, 150, 0, 0, 0, 1, 0, True)
	
	else:
		if put_cup_count < 5 :
			cup_number = 1+(put_cup_count-2)*3
		elif 5 <= put_cup_count < 8 :
			cup_number = 2+(put_cup_count-5)*3
		else :
			cup_number = 3+(put_cup_count-8)*3
		if greencount == 0 :
			if put_cup_count <= 2 :
				return Action('green_right_put', [], [cup_number], [], (300+100*(put_cup_count-1),200), 270, 150, 0, 0, 0, 1, 0, True)
			elif 2 < put_cup_count <= 4 :
				return Action('red_right_put', [], [cup_number], [], (300+100*(put_cup_count-3),500), 270, 150, 0, 0, 0, 1, 0, True)
			elif 4 < put_cup_count <= 7 :
				return Action('red_mid_put', [], [cup_number], [], (300+100*(put_cup_count-3),500), 270, 150, 0, 0, 0, 1, 0, True)
			else:
				return Action('red_left_put', [], [cup_number], [], (300+100*(put_cup_count-3),500), 270, 150, 0, 0, 0, 1, 0, True)
		elif greencount == 1 :
			if put_cup_count <= 3 :
				return Action('green_right_put', [], [cup_number], [], (300+100*(put_cup_count-1),200), 270, 150, 0, 0, 0, 1, 0, True)
			elif 3 < put_cup_count <= 4 :
				return Action('red_right_put', [], [cup_number], [], (300+100*(put_cup_count-4),500), 270, 150, 0, 0, 0, 1, 0, True)
			elif 4 < put_cup_count <= 7 :
				return Action('red_mid_put', [], [cup_number], [], (300+100*(put_cup_count-4),500), 270, 150, 0, 0, 0, 1, 0, True)
			else:
				return Action('red_left_put', [], [cup_number], [], (300+100*(put_cup_count-4),500), 270, 150, 0, 0, 0, 1, 0, True)
		elif greencount == 2 :
			if put_cup_count <= 4 :
				return Action('green_right_put', [], [cup_number], [], (300+100*(put_cup_count-1),200), 270, 150, 0, 0, 0, 1, 0, True)
			elif 4 < put_cup_count <= 7 :
				return Action('red_mid_put', [], [cup_number], [], (300+100*(put_cup_count-5),500), 270, 150, 0, 0, 0, 1, 0, True)
			else:
				return Action('red_left_put', [], [cup_number], [], (300+100*(put_cup_count-5),500), 270, 150, 0, 0, 0, 1, 0, True)
		elif greencount == 3 :
			if put_cup_count <= 4 :
				return Action('green_right_put', [], [cup_number], [], (300+100*(put_cup_count-1),200), 270, 150, 0, 0, 0, 1, 0, True)
			elif 4 < put_cup_count <= 5 :
				return Action('green_mid_put', [], [cup_number], [], (300+100*(put_cup_count-1),200), 270, 150, 0, 0, 0, 1, 0, True)
			elif 5 < put_cup_count <= 7 :
				return Action('red_mid_put', [], [cup_number], [], (300+100*(put_cup_count-6),500), 270, 150, 0, 0, 0, 1, 0, True)
			else:
				return Action('red_left_put', [], [cup_number], [], (300+100*(put_cup_count-6),500), 270, 150, 0, 0, 0, 1, 0, True)
		elif greencount == 4 :
			if put_cup_count <= 4 :
				return Action('green_right_put', [], [cup_number], [], (300+100*(put_cup_count-1),200), 270, 150, 0, 0, 0, 1, 0, True)
			elif 4 < put_cup_count <= 6 :
				return Action('green_mid_put', [], [cup_number], [], (300+100*(put_cup_count-1),200), 270, 150, 0, 0, 0, 1, 0, True)
			elif 6 < put_cup_count <= 7 :
				return Action('red_mid_put', [], [cup_number], [], (300+100*(put_cup_count-7),500), 270, 150, 0, 0, 0, 1, 0, True)
			else:
				return Action('red_left_put', [], [cup_number], [], (300+100*(put_cup_count-7),500), 270, 150, 0, 0, 0, 1, 0, True)
		elif greencount == 5 :
			if put_cup_count <= 4 :
				return Action('green_right_put', [], [cup_number], [], (300+100*(put_cup_count-1),200), 270, 150, 0, 0, 0, 1, 0, True)
			elif 4 < put_cup_count <= 7 :
				return Action('green_mid_put', [], [cup_number], [], (300+100*(put_cup_count-1),200), 270, 150, 0, 0, 0, 1, 0, True)
			else:
				return Action('red_left_put', [], [cup_number], [], (300+100*(put_cup_count-8),500), 270, 150, 0, 0, 0, 1, 0, True)
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
# 0-收納,風標,燈塔 ,1-放置 ,2- ,3- ,4- ,5-底盤
def setting(mode,cup_color):
	demo_path =[]
	global redcount,greencount
	for i in range(5) :
		if color[i]==1 :
			 redcount+=1
	greencount = 5-redcount
	for i in range(5) :
		if color[i]==1 :
			for k in range(i) :
				if(color[i-k-1] != 1):
					color[i-k],color[i-k-1] = color[i-k-1],color[i-k]				
					outside_cup_pos[i-k],outside_cup_pos[i-k-1] = outside_cup_pos[i-k-1],outside_cup_pos[i-k]
	#find_point_order()
	print(outside_cup_pos)
	print(color)
	
	if mode == 1:
		#back to home
		c1 = Action('go_home',[], [], [], (1650, 865), 270, 150, 0, 0, 0, 5, 0, True)

		#grab_cup
			#c2 = grab_fuc()
		c8 = Action('goto_outside_point',[], [], [], (1650, 865), 270, 150, 0, 0, 0, 5, 0, True)
		c9 = Action('goto_inside_point',[], [], [], (1650, 865), 270, 150, 0, 0, 0, 5, 0, True)
	
		#lighthouse
		c4 = Action('hand_up_1',[], [30], [], (1650, 865), 270, 150, 0, 0, 0, 0, 0, True) 
		#c5 = Action('hand_down_1',[], [0], (1650, 865), 3, 270, 150, 1, 15, 5, False, True) 

		#Weathervane
		c6 = Action('hand_up_2',[], [20], [], (1650, 865), 270, 150, 0, 0, 0, 0, 0, True) 
		#c7 = Action('hand_down_2',[], [0], (1650, 865), 3, 270, 150, 1, 15, 5, False, True) #change?
		
		#take_out_cup
			#c3 = put_fuc()
		

		#demo_path1 = [grab_fuc(),grab_fuc(),grab_fuc(),grab_fuc(),grab_fuc(),grab_fuc(),grab_fuc(),grab_fuc(),grab_fuc(),grab_fuc()]
		demo_path = [put_fuc(),put_fuc(),put_fuc(),put_fuc(),put_fuc(),put_fuc(),put_fuc(),put_fuc(),put_fuc(),put_fuc()]
		go_home_path = [c1]

		#print(str(len(demo_path)))
	return demo_path, go_home_path

#setting(1,0)