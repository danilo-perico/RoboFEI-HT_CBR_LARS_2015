#coding: utf-8
#  ----------------------------------------------------------------------------
#  ****************************************************************************
#  * @file talker.py
#  * @author Isaac Jesus da Silva - ROBOFEI-HT - FEI 😛
#  * @version V0.0.1
#  * @created 08/09/2015
#  * @Modified 08/09/2015
#  * @e-mail isaac25silva@yahoo.com.br
#  * @brief Talker 😛
#  ****************************************************************************
#  Program to testing write in the BlackBoard
#  ****************************************************************************
import time
import os
import random
from SharedMemory import SharedMemory as blackboard


bkb = blackboard() #Iniciando a classe do BlackBoard

# Realizando escrita na VISION----------------------------------------------
while True:
	#os.system('clear')
	num1 = 600 # Gerando um número randomico
	num2 = 502 # Gerando um número randomico
	num3 = 2
	num4 = 1
	bkb.write_int("VISION_MOTOR1_ANGLE", num1) #Escrevendo no BlackBoard
	bkb.write_int("VISION_MOTOR2_ANGLE", num2) #Escrevendo no BlackBoard
	bkb.write_int("COM_REFEREE", num3) #Escrevendo no BlackBoard
	bkb.write_int("LOCALIZATION_THETA", num4) #Escrevendo no BlackBoard
	print 'VISION_MOTOR1_ANGLE = %1.4f' % num1
	print 'VISION_MOTOR1_ANGLE = %1.4f' % num2
	print 'com_referee = %1.4f' % num3
	print 'localization_theta = %1.4f' % num4
	time.sleep(1)



#------------------------------------------------------------------------

