#coding: utf-8
#  ----------------------------------------------------------------------------
#  ****************************************************************************
#  * @file decision.py
#   *@project: ROBOFEI-HT - FEI 😛
#  * @author Isaac Jesus da Silva
#  * @version V0.0.1
#  * @created 06/10/2015
#  * @e-mail isaac25silva@yahoo.com.br
#  * @brief Decision
#   *@modified by: Danilo H. Perico
#   *@modified: 14 Oct 2015
#  ****************************************************************************
#  Program to execute the Decision process
#  ****************************************************************************
try:
    from configparser import ConfigParser
except ImportError:
    from ConfigParser import ConfigParser  # ver. < 3.0

#looking for the library SharedMemory
from SharedMemory import SharedMemory as blackboard

bkb = blackboard() #Instantiate the BlackBoard's class

# instantiate
config = ConfigParser()

# looking for the file config.ini
config.read('../../Control/Data/config.ini')

print ('Decision')

#read robot number from config.ini
robot_number = config.getint('Communication','no_player_robofei')
print(robot_number)

# read values from section Offset
head_pan_initial = config.getint('Offset', 'ID_19')
head_tilt_initial = config.getint('Offset', 'ID_20')

print "head_pan = ", head_pan_initial
print "head_tilt = ", head_tilt_initial


#while True:

print "VISION_MOTOR1_ANGLE: ", bkb.read_int('VISION_MOTOR1_ANGLE')
print "VISION_MOTOR2_ANGLE: ", bkb.read_int('VISION_MOTOR2_ANGLE')

