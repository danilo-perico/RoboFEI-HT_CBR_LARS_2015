clear
g++ -I/usr/include/opencv Orientation.cpp -o Orientation -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_flann -Wno-write-strings -lboost_program_options
sleep 2
echo executando
sleep 2
clear
./Vision
