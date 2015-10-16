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

#import parser for arguments    
import argparse

import time

from behavior import *

print
print '################### Decision #########################'
print 



# instantiate
config = ConfigParser()

# looking for the file config.ini
config.read('../../Control/Data/config.ini')

#create arguments for each behavior
parser = argparse.ArgumentParser(description='Robot behavior', epilog= 'Se nenhuma ação for selecionada um comportamento híbrido será adotado! / If there is not a selected argument a hybrid behavior will be adopted!')
parser.add_argument('--golie', '-g', action="store_true", help = 'Seleciona comportamento de goleiro / selects golie behavior')
parser.add_argument('--quarterback', '-q', action="store_true", help = 'Seleciona comportamento de zagueiro / selects quarterback behavior')
parser.add_argument('--attacker', '-a', action="store_true", help = 'Seleciona comportamento de atacante / selects attacker behavior')

args = parser.parse_args()

#Golie decision:
if args.golie == True:
    robot = Golie()
    
#Quarterback decicion:    
elif args.quarterback == True:
    robot = Quarterback()
    
#Attacker decision:    
elif args.attacker == True:
    robot = Attacker()
    
#Hybrid decision:
else:
    robot = Hybrid()

# read values from section Offset
head_pan_initial = config.getint('Offset', 'ID_19')
head_tilt_initial = config.getint('Offset', 'ID_20')

#loop
while True:
    print robot.get_motor_tilt()
    print robot.get_motor_pan()
    time.sleep(2) 
    
    
    
