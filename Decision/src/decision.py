#coding: utf-8
#  ----------------------------------------------------------------------------
#  ****************************************************************************
#  * @file decision.py
#  * @author Isaac Jesus da Silva - ROBOFEI-HT - FEI 😛
#  * @version V0.0.1
#  * @created 06/10/2015
#  * @Modified 06/10/2015
#  * @e-mail isaac25silva@yahoo.com.br
#  * @brief Decision 😛
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

while True:

	# read values from section Offset
	head_pan_initial = config.getint('Offset', 'ID_19')
	head_tilt_initial = config.getint('Offset', 'ID_20')

	print "head_pan = ", head_pan_initial
	print "head_tilt = ", head_tilt_initial

	print "VISION_MOTOR1_ANGLE: ", bkb.read_int('VISION_MOTOR1_ANGLE')
	print "VISION_MOTOR2_ANGLE: ", bkb.read_int('VISION_MOTOR2_ANGLE')

