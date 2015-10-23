blue='\e[0;34m'
NC='\e[0m' # No Color
red='\e[0;31m'
green='\e[0;32m' 

echo
sudo echo  "This script will configure bashrc file and install RoboFEI-HT software"
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
	echo 'export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:'$(pwd)/build/lib >> ~/.bashrc   
	sleep 1
fi


if grep "export PATH" ~/.bashrc; then
	echo -e "${blue} PATH has already been set up... canceling process ${NC}"
else
	echo -e "${blue} PATH Configuration ${NC}"
	#add new path for executables ~/RoboFEI-HT/build/bin
	echo 'export PATH=$PATH:'$(pwd)/build/bin >> ~/.bashrc   
	sleep 1
fi
echo -e "${blue} Installing serial ${NC}"
cd IMU/serial

mkdir build

cd build

cmake ../

make all

sudo make install

cd ../../..

echo -e "${blue} Installing whole software ${NC}"
mkdir build

cd build

cmake ../

make all

make install

sudo echo  -e "Criando as regras para reconhecer o dispositivo${red} IMU${NC}"
cat <<EOF > 41-ftdi-imu.rules
KERNEL=="ttyUSB?", SUBSYSTEMS=="usb", ATTRS{idVendor}=="067b",  ATTRS{idProduct}=="2303", MODE="0777", SYMLINK+="robot/imu"
EOF
chmod +x 41-ftdi-imu.rules
sudo echo  -e "Copiando arquivo${blue} 41-ftdi-imu.rules${NC} para ${green}/etc/udev/rules.d${NC}"
sudo cp  41-ftdi-imu.rules  /etc/udev/rules.d

sudo echo  -e "Criando as regras para reconhecer o dispositivo${red} Servo${NC}"
cat <<EOF > 41-ftdi-servo.rules
KERNEL=="ttyUSB?", SUBSYSTEM=="tty", ATTRS{idVendor}=="0403",  ATTRS{idProduct}=="6001", ATTRS{serial}!="A501VRKI", ATTRS{serial}!="A501VROG", ATTRS{serial}!="A501VM7A", ATTRS{product}=="FT232R USB UART", MODE="0777", SYMLINK+="robot/servo%n"
EOF
chmod +x 41-ftdi-servo.rules
sudo echo  -e "Copiando arquivo${blue} 41-ftdi-servo.rules${NC} para ${green}/etc/udev/rules.d${NC}"
sudo cp 41-ftdi-servo.rules /etc/udev/rules.d

sudo echo  "Realizando um restart no udev"
sudo service udev restart

cd ..

cd  Control/Data/
pwd

sudo echo "*********************************************************"
sudo echo "Preparando para copiar arquivos de configuração do robô" 
sudo echo "Informe o numero do robô: "
read NUM
case $NUM in
  1) echo "Robô 1 selecionado..."
     cp ../../conf_robos/01/* .
     echo "Arquivos copiados!"   ;;

  2) sudo echo "Robô 2 selecionado..."
     cp ../../conf_robos/02/* .
     echo "Arquivos copiados!"     ;;

  3) sudo echo "Robô 3 selecionado..."
     cp ../../conf_robos/03/* .
     echo "Arquivos copiados!"     ;;

  4) sudo echo "Robô 4 selecionado..."
     cp ../../conf_robos/04/* .
     echo "Arquivos copiados!"     ;;
  
  *) sudo echo "Numero inválido!" ;;
esac

echo -e "${red} Please close terminal in order to apply changes ${NC}" 
# reboot
#sleep 5
#kill -TERM `pidof gnome-terminal`

