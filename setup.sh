blue='\e[0;34m'
NC='\e[0m' # No Color
red='\e[0;31m'

echo
echo  "This script will configure bashrc file and install RoboFEI-HT software"
echo
read -p "Continue? (y/n) " -n 1 -r
echo 
if [[  $REPLY =~ ^[Nn]$ ]]
then
    exit 1
fi


if grep "LD_LIBRARY" ~/.bashrc; then
	echo -e "${blue}  LD_LIBRARY_PATH has already been set up... canceling process ${NC}"
else
	echo -e "${blue}  LD_LIBRARY_PATH Configuration ${NC}"
	#add new path ~/RoboFEI-HT/build/lib	
	echo 'export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/RoboFEI-HT/build/lib' >> ~/.bashrc   
	sleep 1
fi


if grep "export PATH" ~/.bashrc; then
	echo -e "${blue} PATH has already been set up... canceling process ${NC}"
else
	echo -e "${blue} PATH Configuration ${NC}"
	#add new path for executables ~/RoboFEI-HT/build/bin
	echo 'export PATH=$PATH:~/RoboFEI-HT/build/bin' >> ~/.bashrc   
	sleep 1
fi

mkdir build

cd build

cmake ../

make all

make install

echo -e "${red} Please close terminal in order to apply changes ${NC}" 
#sleep 5
#kill -TERM `pidof gnome-terminal`

