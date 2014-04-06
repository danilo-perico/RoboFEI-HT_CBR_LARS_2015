#include <dynamixel.h>

void delay_mov(unsigned int delta_posicao, unsigned int velocidade)
{
unsigned int delay; // tempo total para percorrer caminho informado

// 538.421 tempo em segundos que o servo leva par completar uma volta na velocidade 1
// 1228.8 definição de 1 volta completa -> pq 1023 = 300° portanto 1228.8 = 360°
delay = ((538.421/velocidade)*delta_posicao)/1228.8;

usleep(delay*1000000);

}
