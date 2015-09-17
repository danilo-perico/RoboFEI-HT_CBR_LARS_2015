#!/bin/bash
#!/RoboFEI-HT/build/bin

echo "communication"
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/RoboFEI-HT/build/lib
export PATH=$PATH:~/RoboFEI-HT/build/bin
source #HOME/.bashrc
CommClient
