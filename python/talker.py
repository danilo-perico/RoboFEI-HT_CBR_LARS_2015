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

bkb.write_int("PLANNING_COMMAND", 12)  # Escrevendo no Blackboard
print bkb.read_int("PLANNING_COMMAND") #Lendo do BlackBoard

# Realizando escrita na IMU----------------------------------------------
while True:
	os.system('clear')
	num1 = random.uniform(0, 1.00) # Gerando um número randomico
	num2 = random.uniform(0, 1.00) # Gerando um número randomico
	num3 = random.uniform(0, 1.00) # Gerando um número randomico
	bkb.write_float("IMU_ACCEL_X", num1) #Escrevendo no BlackBoard
	bkb.write_float("IMU_ACCEL_Y", num2) #Escrevendo no BlackBoard
	bkb.write_float("IMU_ACCEL_Z", num3) #Escrevendo no BlackBoard
	print "Escrevendo no IMU_ACCEL_X = %1.4f" % num1
	print "Escrevendo no IMU_ACCEL_Y = %1.4f" % num2
	print "Escrevendo no IMU_ACCEL_Z = %1.4f" % num3
	time.sleep(1)
#------------------------------------------------------------------------

