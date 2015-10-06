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

# instantiate
config = ConfigParser()

# looking for the file config.ini
config.read('../../Control/Data/config.ini')

# read values from section Offset
head_pan = config.getint('Offset', 'ID_19')
head_tilt = config.getint('Offset', 'ID_20')


print "head_pan = ", head_pan
print "head_tilt = ", head_tilt

