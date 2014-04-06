#include <dynamixel.h>

void robo_ereto()
{
    for(int i = 1;i<23;i++)
    {
       dxl_write_word(i, MOVING_SPEED, 100);
       dxl_write_word(i, P_GOAL_POSITION_L, StandupPos[i-1]);
    }
}
//================================================================================
void robo_ereto_corrigido()
{
    for(int i = 1;i<23;i++)
    {
       dxl_write_word(i, MOVING_SPEED, 100);
       dxl_write_word(i, P_GOAL_POSITION_L, StandupPos_corrigido[i-1]);
    }
}
