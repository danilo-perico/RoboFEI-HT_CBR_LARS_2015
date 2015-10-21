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

try:
    from configparser import ConfigParser
except ImportError:
    from ConfigParser import ConfigParser  # ver. < 3.0

#looking for the library SharedMemory
from SharedMemory import SharedMemory 

class TreatingRawData(object):

     #Instantiate the BlackBoard's class:
    bkb = SharedMemory()
    
    # instantiate:
    config = ConfigParser()

    # looking for the file config.ini:
    config.read('../../Control/Data/config.ini')
    
    def __init__(self):
        print
        print 'Raw data - read (get) and write (set) methods'
        print
        
    def get_referee_usage(self):
        return self.config.getint('Decision', 'referee')
                    
    def get_referee(self):
        return self.bkb.read_int('COM_REFEREE')

    def get_motor_tilt(self):
        return self.bkb.read_int('VISION_MOTOR1_ANGLE')
        
    def get_motor_pan(self):
        return self.bkb.read_int('VISION_MOTOR2_ANGLE')
          
    def get_orientation(self):
        return self.bkb.read_int('LOCALIZATION_THETA')
        
    def get_dist_ball(self):
        return self.bkb.read_int('VISION_DIST_BALL')           
        
    def get_head_pan_initial(self):
        return self.config.getint('Offset', 'ID_19')
        
    def get_search_ball_status(self):
        return self.bkb.read_int('VISION_SEARCH_BALL')
        
    def get_lost_ball_status(self):
        return self.bkb.read_int('VISION_LOST_BALL')
        
    def get_head_tilt_initial(self):
        return self.config.getint('Offset', 'ID_20')
        
    def set_gait(self):
        return self.bkb.write_int('DECISION_ACTION_A', 0)

    def set_walk_forward(self):
        return self.bkb.write_int('DECISION_ACTION_A', 1)

    def set_turn_left(self):
        return self.bkb.write_int('DECISION_ACTION_A', 2)        
        
    def set_turn_right(self):
        return self.bkb.write_int('DECISION_ACTION_A', 3)       
        
    def set_kick_right(self):
        return self.bkb.write_int('DECISION_ACTION_A', 4)       
        
    def set_kick_left(self):
        return self.bkb.write_int('DECISION_ACTION_A', 5)       
        
    def set_sidle_left(self):
        return self.bkb.write_int('DECISION_ACTION_A', 6)       
        
    def set_sidle_right(self):
        return self.bkb.write_int('DECISION_ACTION_A', 7)       
        
    def set_walk_forward_slow(self):
        return self.bkb.write_int('DECISION_ACTION_A', 8)       
        
    def set_revolve_around_ball(self):
        return self.bkb.write_int('DECISION_ACTION_A', 9)       
        
    def set_walk_backward(self):
        return self.bkb.write_int('DECISION_ACTION_A', 10)       
        
    def set_stand_still(self):
        return self.bkb.write_int('DECISION_ACTION_A', 11)       
        
    def set_vision_ball(self):
        return self.bkb.write_int('DECISION_ACTION_VISION',0)
        
    def set_vision_orientation(self):
        return self.bkb.write_int('DECISION_ACTION_VISION',2) 
        


class Ordinary(TreatingRawData):
    " " " Ordinary class " " "
    
    def __init__(self):
        print
        print 'Ordinary behavior called' 
        print
        
    def decision(self, referee):
        if referee == 1: #stopped
            print 'stand'
            self.set_stand_still()
            
        elif referee == 11: #ready
            print 'ready'
            self.set_stand_still()
            
        elif referee == 12: #set
            print 'set' 
            self.set_stand_still()
            self.set_vision_ball()
            
        elif referee == 2: #play
            print 'play'
            self.set_vision_ball()
            if self.get_search_ball_status == 1:
                self.set_stand_still()
                if self.get_lost_ball_status == 1:
                   self.set_turn_right() 
            

    
############################        
        
class Attacker(Ordinary):
    " " " Attacker class " " "

    def __init__(self):
        print
        print  'Attacker behavior called' 
        print

############################        
        
class Quarterback(Ordinary):
    " " " Quarterback class " " "

    def __init__(self):
        print
        print  'Quarterback behavior called' 
        print
        
############################ 
        
class Golie(Ordinary):
    " " " Golie class " " "

    def __init__(self):
        print
        print  'Golie behavior called' 
        print
        
    def set_jump_left(self):
        return self.bkb.write_int('DECISION_ACTION_A', 13)  

    def set_jump_right(self):
        return self.bkb.write_int('DECISION_ACTION_A', 14)  
        
