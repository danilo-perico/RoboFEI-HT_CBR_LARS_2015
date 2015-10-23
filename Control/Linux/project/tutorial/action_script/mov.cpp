

#include <iostream>
#include <stdio.h>
#include "mov.hpp"

TurnBall::TurnBall(minIni* ini)
{
   	//Carregando valores do config.ini na section Turn Ball
 	if((andar_X=ini->getd("Turn Ball","andar_X",-1024))==-1024){
		std::cout<<"Erro na leitura do conf.ini section:Turn Ball andar_X"<<std::endl;
		andar_X=0;
	}	    
    else if(andar_X>30 || andar_X<-30) // limita o range
	    {andar_X=0; std::cout<<"Valor do section:Turn Ball andar_X no conf.ini fora do range"<<std::endl;}  	
   	
	if((turn_angle=ini->getd("Turn Ball","turn_angle",-1024))==-1024){
		std::cout<<"Erro na leitura do conf.ini section:Turn Ball turn_angle"<<std::endl;
		turn_angle=-10;
	}	    
    else if(turn_angle>30 || turn_angle<-30) // limita o range
	    {turn_angle=-10; std::cout<<"Valor do section:Turn Ball turn_angle no conf.ini fora do range"<<std::endl;}
	    
	if((andar_lateral=ini->getd("Turn Ball","andar_lateral",-1024))==-1024){
		std::cout<<"Erro na leitura do conf.ini section:Turn Ball andar_lateral"<<std::endl;
		andar_lateral=15;
	}	    
    else if(andar_lateral>50 || andar_lateral<-50) // limita o range
	    {andar_lateral=15; std::cout<<"Valor do section:Turn Ball andar_lateral no conf.ini fora do range"<<std::endl;}
	    
	if((z_offset=ini->getd("Turn Ball","z_offset",-1024))==-1024){
		std::cout<<"Erro na leitura do conf.ini section:Turn Ball z_offset"<<std::endl;
		z_offset=25;
	}

	if((pitch_offset=ini->getd("Turn Ball","pitch_offset",-1024))==-1024){
		std::cout<<"Erro na leitura do conf.ini section:Turn Ball pitch_offset"<<std::endl;
		pitch_offset=0;
	}	
	
	if((period_time=ini->getd("Turn Ball","period_time",-1024))==-1024){
		std::cout<<"Erro na leitura do conf.ini section:Turn Ball period_time"<<std::endl;
		period_time=600;
	}

	if((dsp_ratio=ini->getd("Turn Ball","dsp_ratio",-1024))==-1024){
		std::cout<<"Erro na leitura do conf.ini section:Turn Ball dsp_ratio"<<std::endl;
		dsp_ratio=0.125;
	}
		

	if((foot_height=ini->getd("Turn Ball","foot_height",-1024))==-1024){
		std::cout<<"Erro na leitura do conf.ini section:Turn Ball foot_height"<<std::endl;
		foot_height=30;
	}	

	if((swing_right_left=ini->getd("Turn Ball","swing_right_left",-1024))==-1024){
		std::cout<<"Erro na leitura do conf.ini section:Turn Ball swing_right_left"<<std::endl;
		swing_right_left=15;
	}

	if((swing_top_down=ini->getd("Turn Ball","swing_top_down",-1024))==-1024){
		std::cout<<"Erro na leitura do conf.ini section:Turn Ball swing_top_down"<<std::endl;
		swing_top_down=8;
	}
	
   	//Carregando valores do config.ini na section Walking Config
	if((c_z_offset=ini->getd("Walking Config","z_offset",-1024))==-1024){
		std::cout<<"Erro na leitura do conf.ini section:Walking Config z_offset"<<std::endl;
		c_z_offset=25;
	}

	if((c_pitch_offset=ini->getd("Walking Config","pitch_offset",-1024))==-1024){
		std::cout<<"Erro na leitura do conf.ini section:Walking Config pitch_offset"<<std::endl;
		c_pitch_offset=0;
	}	
	
	if((c_period_time=ini->getd("Walking Config","period_time",-1024))==-1024){
		std::cout<<"Erro na leitura do conf.ini section:Walking Config period_time"<<std::endl;
		c_period_time=600;
	}	


	if((c_dsp_ratio=ini->getd("Walking Config","dsp_ratio",-1024))==-1024){
		std::cout<<"Erro na leitura do conf.ini section:Walking Config dsp_ratio"<<std::endl;
		c_dsp_ratio=600;
	}	


	if((c_foot_height=ini->getd("Walking Config","foot_height",-1024))==-1024){
		std::cout<<"Erro na leitura do conf.ini section:Walking Config foot_height"<<std::endl;
		c_foot_height=30;
	}	

	if((c_swing_right_left=ini->getd("Walking Config","swing_right_left",-1024))==-1024){
		std::cout<<"Erro na leitura do conf.ini section:Walking Config swing_right_left"<<std::endl;
		c_swing_right_left=15;
	}

	if((c_swing_top_down=ini->getd("Walking Config","swing_top_down",-1024))==-1024){
		std::cout<<"Erro na leitura do conf.ini section:Walking Config swing_top_down"<<std::endl;
		c_swing_top_down=8;
	}
}


void TurnBall::updateTurnValue(Walking* walk)
{
    walk->PERIOD_TIME = period_time;
    walk->DSP_RATIO = dsp_ratio;
    walk->Z_OFFSET = z_offset;
    walk->P_OFFSET = pitch_offset;
    walk->Z_MOVE_AMPLITUDE = foot_height;
    walk->Y_SWAP_AMPLITUDE = swing_right_left;
    walk->Z_SWAP_AMPLITUDE = swing_top_down;
    
    walk->update_param_time();
    walk->update_param_balance();
    walk->update_param_move();
}


void TurnBall::updateConfig(Walking* walk)
{
    walk->PERIOD_TIME =      c_period_time;
    walk->DSP_RATIO =        c_dsp_ratio;
    walk->Z_OFFSET =         c_z_offset;
    walk->P_OFFSET =         c_pitch_offset;
    walk->Z_MOVE_AMPLITUDE = c_foot_height;
    walk->Y_SWAP_AMPLITUDE = c_swing_right_left;
    walk->Z_SWAP_AMPLITUDE = c_swing_top_down;
    
    walk->update_param_time();
    walk->update_param_balance();
    walk->update_param_move();
}
