#include <dynamixel.h>


//---- Função que trava todos os servos ------------------------------------------
void travar_servos()
{
    for(int i = 1;i<23;i++)
         dxl_write_word(i, 24, 1);
}
//================================================================================
