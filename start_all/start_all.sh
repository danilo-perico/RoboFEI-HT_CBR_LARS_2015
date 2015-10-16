#!/bin/bash
#!/RoboFEI-HT/build/bin

sudo echo "starting all processes"
# export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/RoboFEI-HT/build/lib
# export PATH=$PATH:~/RoboFEI-HT/build/bin
# source #HOME/.bashrc

while true
do
    if [ ! "$(pidof imu)" ] 
    then
       gnome-terminal -x sh -c './start_imu.sh' &
   fi
#   if [ ! "$(pidof -x start_vision.sh)" ] 
#   then
#          gnome-terminal -x sh -c 'echo 123456 | sudo -S ./start_vision.sh'
#   fi
    if [ ! "$(pidof -x start_decision.sh)" ]  
    then
        gnome-terminal -x sh -c './start_decision.sh'
    fi
#    if [ ! "$(pidof control)" ] 
#    then
#       gnome-terminal -x sh -c 'echo 123456 | sudo -S ./start_control.sh'
#    fi
#    if [ ! "$(pidof communication)" ] 
#    then
#       gnome-terminal -x sh -c 'echo 123456 | sudo -S ./start_comm.sh'
#    fi
#    if [ ! "$(pidof commServer)" ] 
#    then
#       gnome-terminal -x sh -c 'echo 123456 | sudo -S ./start_comm.sh'
#    fi
#    if [ ! "$(pidof CommClient)" ] 
#    then
#       gnome-terminal -x sh -c 'echo 123456 | sudo -S ./start_comm.sh'
#    fi
    sleep 10
done
