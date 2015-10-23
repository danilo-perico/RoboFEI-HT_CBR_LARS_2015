

#include <iostream>
#include <stdio.h>
#include "mov.hpp"

TurnBall::TurnBall(minIni* ini)
{
   	//Carregando valores do config.ini na section Turn Ball
	if((turn_angle=ini->getd("Turn Ball","turn_angle",-1024))==-1024){
		std::cout<<"Erro na leitura do conf.ini section:Turn Ball turn_angle"<<std::endl;
		turn_angle=-10;
	}	    
    else if(turn_angle>30 || turn_angle<-30) // limita o range
	    turn_angle=-10;
	    
	if((andar_lateral=ini->getd("Turn Ball","andar_lateral",-1024))==-1024){
		std::cout<<"Erro na leitura do conf.ini section:Turn Ball andar_lateral"<<std::endl;
		andar_lateral=15;
	}	    
    else if(andar_lateral>30 || andar_lateral<-30) // limita o range
	    andar_lateral=15;
}


void updateTurnValue()
{

}


void updateConfig()
{

}
