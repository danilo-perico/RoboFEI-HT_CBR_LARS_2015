/*--------------------------------------------------------------------

******************************************************************************
  * @file       blackboard.h
  * @author     Isaac Jesus da Silva - ROBOFEI-HT - FEI
  * @version    V0.0.0
  * @created    07/04/2014
  * @Modified   07/04/2014
  * @e-mail     isaac25silva@yahoo.com.br
  * @brief      Main header black board
  ****************************************************************************

  Arquivo de cabeçalho contendo as funções e definições do black board

/--------------------------------------------------------------------*/

//---- Definições da memória compartilhada------------------------------
#define PLANNING_COMMAND *(mem)
#define PLANNING_PARAMETER_VEL *(mem+1)
#define PLANNING_PARAMETER_ANGLE *(mem+2)
#define IMU_STATE *(mem+3)
#define IMU_GYRO_X *(mem+4)
#define IMU_GYRO_Y *(mem+5)
#define IMU_GYRO_Z *(mem+6)
#define IMU_ACCEL_X *(mem+7)
#define IMU_ACCEL_Y *(mem+8)
#define IMU_ACCEL_Z *(mem+9)
#define IMU_COMPASS_A *(mem+10)
#define IMU_COMPASS_B *(mem+11)
#define IMU_COMPASS_C *(mem+12)
#define CONTROL_ACTION *(mem+13)
#define CONTROL_HEIGHT_A *(mem+14)
#define CONTROL_HEIGHT_B *(mem+15)
#define CONTROL_HEIGHT_C *(mem+16)
#define DECISION_ACTION_A *(mem+17)
#define DECISION_ACTION_B *(mem+18)
#define DECISION_STATE *(mem+19)
#define DECISION_POSITION_A *(mem+20)
#define DECISION_POSITION_B *(mem+21)
#define DECISION_POSITION_C *(mem+22)
#define DECISION_BALL_POS *(mem+23)
#define DECISION_OPP1_POS *(mem+24)
#define DECISION_OPP2_POS *(mem+25)
#define DECISION_OPP3_POS *(mem+26)
#define COM_ACTION_ROBOT1 *(mem+27)
#define COM_ACTION_ROBOT2 *(mem+28)
#define COM_ACTION_ROBOT3 *(mem+29)
#define COM_STATE_ROBOT1 *(mem+30)
#define COM_STATE_ROBOT2 *(mem+31)
#define COM_STATE_ROBOT3 *(mem+32)
#define COM_POS_ROBOT1 *(mem+33)
#define COM_POS_ROBOT2 *(mem+34)
#define COM_POS_ROBOT3 *(mem+35)
#define COM_POS_BALL_ROBOT1 *(mem+36)
#define COM_POS_BALL_ROBOT2 *(mem+37)
#define COM_POS_BALL_ROBOT3 *(mem+38)
#define COM_POS_OPP_A_ROBOT1 *(mem+39)
#define COM_POS_OPP_A_ROBOT2 *(mem+40)
#define COM_POS_OPP_A_ROBOT3 *(mem+41)
#define COM_POS_OPP_A_ROBOT4 *(mem+42)
#define COM_POS_OPP_B_ROBOT1 *(mem+43)
#define COM_POS_OPP_B_ROBOT2 *(mem+44)
#define COM_POS_OPP_B_ROBOT3 *(mem+45)
#define COM_POS_OPP_B_ROBOT4 *(mem+46)
#define COM_POS_OPP_C_ROBOT1 *(mem+47)
#define COM_POS_OPP_C_ROBOT2 *(mem+48)
#define COM_POS_OPP_C_ROBOT3 *(mem+49)
#define COM_POS_OPP_C_ROBOT4 *(mem+50)
#define COM_REFEREE *(mem+51)
#define LOCALIZATION_X *(mem+52)
#define LOCALIZATION_Y *(mem+53)
#define LOCALIZATION_THETA *(mem+54)
#define VISION_DIST_BALL *(mem+55)
#define VISION_DIST_GOAL *(mem+56)
#define VISION_DIST_OPP1 *(mem+57)
#define VISION_DIST_OPP2 *(mem+58)
#define VISION_DIST_OPP3 *(mem+59)

//----global variables------------------------------------------------
extern int *mem ; //Variável que manipula memória compartilhada

//----Functions prototype---------------------------------------------
int using_shared_memory(); //Função que cria e acopla a memória compartilhada


