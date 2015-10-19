#coding: utf-8
 # -----------------------------------------------------------------------------------------------------
 # ****************************************************************************
 # * @file behavior.py
 # * @author Danilo H Perico
 # * @ROBOFEI-HT - FEI 😛
 # * @created 15/10/2015
 # * @e-mail danilo.perico@gmail.com
 # * @brief robot behaviors
 # ****************************************************************************

#looking for the library SharedMemory
from SharedMemory import SharedMemory 


class Hybrid(object):
    " " " Hybrid class " " "
    
     #Instantiate the BlackBoard's class
    bkb = SharedMemory()

    def __init__(self):
        print
        print 'Hybrid behavior called' 
        print

    def get_motor_tilt(self):
        return self.bkb.read_int('VISION_MOTOR1_ANGLE')
        
    def get_motor_pan(self):
        return self.bkb.read_int('VISION_MOTOR2_ANGLE')
        
        
############################        
        
class Attacker(Hybrid):
    " " " Attacker class " " "

    def __init__(self):
        print
        print  'Attacker behavior called' 
        print

############################        
        
class Quarterback(Hybrid):
    " " " Quarterback class " " "

    def __init__(self):
        print
        print  'Quarterback behavior called' 
        print
        
############################ 
        
class Golie(Hybrid):
    " " " Golie class " " "

    def __init__(self):
        print
        print  'Golie behavior called' 
        print
        
