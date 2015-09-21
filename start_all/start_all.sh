#!/bin/bash
#!/RoboFEI-HT/build/bin

echo "start all"
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/RoboFEI-HT/build/lib
export PATH=$PATH:~/RoboFEI-HT/build/bin
source #HOME/.bashrc
while true
do
   if [ ! "$(pidof imu)" ] 
   then
      gnome-terminal -x sh -c 'echo 123456 | sudo -S ./home/fei/RoboFEI-HT/start_all/start_imu.sh' &
   fi
   if [ ! "$(pidof vision)" ] 
   then
      gnome-terminal -x sh -c 'echo 123456 | sudo -S ./home/fei/RoboFEI-HT/start_all/start_vision.sh' &
   fi
   if [ ! "$(pidof decision)" ] 
   then
      gnome-terminal -x sh -c 'echo 123456 | sudo -S ./home/fei/RoboFEI-HT/start_all/start_decision.sh' &
   fi
   if [ ! "$(pidof control)" ] 
   then
      gnome-terminal -x sh -c 'echo 123456 | sudo -S ./home/fei/RoboFEI-HT/start_all/start_control.sh' &
   fi
   if [ ! "$(pidof communication)" ] 
   then
      gnome-terminal -x sh -c 'echo 123456 | sudo -S ./home/fei/RoboFEI-HT/start_all/start_comm.sh' &
   fi
   if [ ! "$(pidof commServer)" ] 
   then
      gnome-terminal -x sh -c 'echo 123456 | sudo -S ./home/fei/RoboFEI-HT/start_all/start_comm.sh' &
   fi
   if [ ! "$(pidof CommClient)" ] 
   then
      gnome-terminal -x sh -c 'echo 123456 | sudo -S ./home/fei/RoboFEI-HT/start_all/start_comm.sh' &
   fi
   sleep 10
done