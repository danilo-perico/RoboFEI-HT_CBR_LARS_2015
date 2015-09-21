#coding: utf-8
 # ----------------------------------------------------------------------------
 # ****************************************************************************
 # * @file listener.py
 # * @author Isaac Jesus da Silva - ROBOFEI-HT - FEI 😛
 # * @version V0.0.1
 # * @created 08/09/2015
 # * @Modified 08/09/2015
 # * @e-mail isaac25silva@yahoo.com.br
 # * @brief Listener 😛
 # ****************************************************************************
 # Program to test the BlackBoard reading
 # ****************************************************************************
import time
import os
from SharedMemory import SharedMemory as blackboard

bkb = blackboard() #Iniciando a classe do BlackBoard

# Realizando leitura na IMU----------------------------------------------
while True:
	os.system('clear')
	print "Lendo do IMU_ACCEL_X = %1.4f" % bkb.read_float("IMU_ACCEL_X")
	print "Lendo do IMU_ACCEL_Y = %1.4f" % bkb.read_float("IMU_ACCEL_Y")
	print "Lendo do IMU_ACCEL_Z = %1.4f" % bkb.read_float("IMU_ACCEL_Z")
	time.sleep(1)
#------------------------------------------------------------------------

