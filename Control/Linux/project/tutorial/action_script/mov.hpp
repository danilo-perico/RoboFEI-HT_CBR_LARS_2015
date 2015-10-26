/*--------------------------------------------------------------------

******************************************************************************
* @file mov.hpp
* @author Isaac Jesus da Silva - ROBOFEI-HT - FEI 😛
* @version V1.0.5
* @created 23/09/2015
* @Modified 23/10/2015
* @e-mail isaac25silva@yahoo.com.br
* @brief mov 😛
****************************************************************************
**************************************************************************** 
Arquivo cabeçalho contendo metodos de movimentos de acoes e gerador de caminhada
/--------------------------------------------------------------------------*/

#ifndef MOV_H
#define MOV_H

#include "minIni.h"
#include "Walking.h"

using namespace Robot;

// Criação da Classe
class Move
{
    	public:

    	/*!
    	* Construtor.
    	*/
    	//Move(){};
    	/*! Destrutor */
    	//~Matriz();

        /*!Método que realiza a multiplicação entre 2 matrizes.
	* @param x - Paramentro de entrada  x.
	* @param y - Paramentro de entrada  x.
	* @param z - Retorna na matriz z o resultado da multiplicação entre as matrizes x e y.
        * @return - Não retorna valores.
        */
//    	void mult()
//		{

//		}
};

class TurnBall
{

    public:
    
    	/*!
    	* Construtor.
    	*/
    	TurnBall(minIni* ini);
    
        double andar_X;
        double turn_angle;
        double andar_lateral;
        double z_offset;
        double pitch_offset;
        double period_time;
        double dsp_ratio;
        double foot_height;
        double swing_right_left;
        double swing_top_down;
        
        double c_z_offset;
        double c_pitch_offset;
        double c_period_time;
        double c_dsp_ratio;
        double c_foot_height;
        double c_swing_right_left;
        double c_swing_top_down;
        
        void updateTurnValue(Walking* walk);
        void updateConfig(Walking* walk);
        
        
};

//*********************************************************************
//---------------------------------------------------------------------

#endif
