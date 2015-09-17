21CRobotFramework
=================

21st Century Robot Framework is a collaborative effort based on the open source Darwin-OP Framework, with the intent to continue development, implement an API to expose higher level functions of the framework, and push additional features. 

Contributions & authoring to codebase provided by Trossen Robotics, Farrell Robotics, AlterRobotics, Zerom, Intel Corp, ROBOTIS, KevinO, KurtE. A special thanks to Farrell Robotics, a massive contributor and resource for humanoid robots & additions to the Darwin-OP framework. 

This is an early push and is missing major features such as a REST API to expose higher level functions of the framework library. 

All code within project is GPL GNU v3.

Original source code can be found here:
https://sourceforge.net/projects/darwinop/

Maintainer
=================
* Name: Andrew Alter
* Email: andrew@trossenrobotics.com
* Github: AlterRobotics
* Website: http://www.trossenrobotics.com

Notes
==================
* Operating systems supported & verified: Ubuntu 12.04 LTS, Yocto Poky 1.6 OpenEmbedded Linux
* This modified framework currently supports the CM-730 Subcontroller from Robotis, but runs a custom firmware version authored by Farrell Robotics. Please update firmware of your CM-730 to latest version 13 available in /Linux/project/firmware_installer
* ps3_demo has no state engine for switching between walking, action, and sitting modes. Read through code carefully and be EXTREMELY cautious with button commands, as they may cause the robot to become unstable. DO NOT execute any button commands from a sitting position. Triangle button initializes into Walk-Ready, which can then be used as a starting point for action motions. PS3 button layout coming soon.
* ps3_demo does not currently work for BlueZ5. For BlueZ4, sixad -s must be launched at startup (ie: rc.local). Requires pairing via sixpair. See Wiki for more information. PS3controller library authored by Farrell Robotics.
* rme (robot motion editor) is an improved version of action_editor,  authored by Farrell Robotics. Additional features such as individual limb on/off torque control implemented (see wiki). Robot MUST be in sitting position and/or spotted when launching rme, as servos go into low-torque mode upon launching rme followed by the robot sitting down. Currently rme is the only way to create motion pages/files.
* Autonomous blob tracking demos/Vision modules from original Darwin-OP framework have been disabled.

TODO
=================
* js.node REST based API & Intel XDK app support, coming soon.
* Finish cmd_demo for Arbotix Commander interface example program
* ps3_demo (USE WITH CAUTION) needs mode check for sitting, button layout documentation. 
* Update firmware_installer with proper MX-106/MX-64T firmware binaries and support.
* MX-28 library update to MX-64/106, support for multiple servo types.
* Adjust torque scaling with voltage.
* Support for Arbotix-PRO replacement for CM-730 subcontroller



Ubuntu Build Notes
==================
apt-get should have these

Core library dependencies:
build-essential libncurses5-dev libjpeg-dev mplayer mplayer-skins 

PS3/Bluetooth dependencies:
bluez-utils bluez-compat bluez-hcidump libusb-dev libbluetooth-dev joystick


Make Instructions
=================
'make all' in /Linux/build to compile core framework libraries.

'make clean && make all' in /Linux/build any time changes are made to core libraries.

demos & utils are found in /Linux/project 'make clean && make all' for all /project programs


Yocto/meta-21crobot instructions
=================

To regenerate the bitbake recipe:
---------------------------------

In the top-level 21CRobotFramework directory, run:  
    make -C Recipe clean  
    make -C Recipe  

Then copy 21CRobotFramework/Recipe/robot21c into meta-21crobot/recipes-core. 

You should do this whenever the source code of 21CRobotFramework changes, and be sure to
commit the changed version of the recipe into the meta-21crobot repository as well.

/Data still needs to be manually copied over to the target file system.

