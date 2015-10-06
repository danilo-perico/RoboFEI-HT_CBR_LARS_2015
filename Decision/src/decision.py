try:
    from configparser import ConfigParser
except ImportError:
    from ConfigParser import ConfigParser  # ver. < 3.0

# instantiate
config = ConfigParser()

# looking for the file config.ini
config.read('../../Control/Data/config.ini')

# read values from section Offset
head_pan = config.get('Offset', 'ID_19')
head_tilt = config.getboolean('Offset', 'ID_20')


print "head_pan = ", head_pan
print "head_tilt = ", head_tilt

