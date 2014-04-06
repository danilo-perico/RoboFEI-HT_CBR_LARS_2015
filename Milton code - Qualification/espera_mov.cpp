#include <dynamixel.h>
#include <unistd.h>

void espera_mov()
{
    for(int i=3;i<23;i++)
    {
       do
       {
        usleep(2000); //Espera 2 milisegundos evitando uso excessivo do processador
       }while(dxl_read_byte( i, P_MOVING ) != 0);
    }
}


void espera_mov_ID(int id)
{
       do
       {
        usleep(2000); //Espera 2 milisegundos evitando uso excessivo do processador
       }while(dxl_read_byte( id, P_MOVING ) != 0);
}
