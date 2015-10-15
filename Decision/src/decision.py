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

#looking for the library SharedMemory
from SharedMemory import SharedMemory as blackboard

print('')
print ('################### Decision #########################')
print('')

bkb = blackboard() #Instantiate the BlackBoard's class
print('')
# instantiate
config = ConfigParser()
print('')
# looking for the file config.ini
config.read('../../Control/Data/config.ini')

parser = argparse.ArgumentParser(description='Robot behavior', epilog= 'Se nenhuma ação for selecionada um comportamento híbrido será adotado! / If there is not a selected argument a hybrid behavior will be adopted!')
parser.add_argument('--golie', '-g', action="store_true", help = 'Seleciona comportamento de goleiro / selects golie behavior')
parser.add_argument('--quarterback', '-q', action="store_true", help = 'Seleciona comportamento de zagueiro / selects quarterback behavior')
parser.add_argument('--attacker', '-a', action="store_true", help = 'Seleciona comportamento de atacante / selects attacker behavior')

args = parser.parse_args()

#Golie decision:
if args.golie == True:
    print('')
    print('----------- Golie behavior ----------------')
    print('')

#Quarterback decicion:    
elif args.quarterback == True:
    print('')
    print('----------- Quarterback behavior ----------------')
    print('')
    
#Attacker decision:    
elif args.attacker == True:
    print('')
    print('----------- Attacker behavior ----------------')
    print('')

#Hybrid decision:
else:
    print('')
    print('----------- Hybrid behavior ----------------')
    print('')
    

#read robot number from config.ini

# read values from section Offset
head_pan_initial = config.getint('Offset', 'ID_19')
head_tilt_initial = config.getint('Offset', 'ID_20')

#print "head_pan = ", head_pan_initial
#print "head_tilt = ", head_tilt_initial

while True:



    bkb.read_int('VISION_MOTOR1_ANGLE')
    bkb.read_int('VISION_MOTOR2_ANGLE')

