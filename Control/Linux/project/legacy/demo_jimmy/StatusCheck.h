/*
 * StatusCheck.h
 *
 *  Created on: 2011. 1. 21.
 *      Author: zerom
 */

#ifndef STATUSCHECK_H_
#define STATUSCHECK_H_

#include "CM730.h"
#include "minIni.h"

#define SCRIPT_FILE_PATH    "script.asc"
#define SCRIPT1_FILE_PATH    "script1.asc"
#define SCRIPT2_FILE_PATH    "script2.asc"
#define SCRIPT3_FILE_PATH    "script3.asc"
#define SCRIPT4_FILE_PATH    "script4.asc"
#define SCRIPT5_FILE_PATH    "script5.asc"
#define SCRIPT6_FILE_PATH    "script6.asc"
#define SCRIPT7_FILE_PATH    "script7.asc"
#define SCRIPT8_FILE_PATH    "script8.asc"



namespace Robot
{
	enum {
        INITIAL,
        READY,
				SITTING,
        SOCCER,
				LINE_FOLLOWING,
				ROBOT_FOLLOWING,
        MOTION,
        VISION,
				STAIRS,
        MAX_MODE
    };

    enum {
        BTN_MODE = 1,
        BTN_START = 2
    };

		enum {
			FAST_WALK,
			MEDIUM_WALK,
			SLOW_WALK
		};

		enum {
			WAIT,
			DONT_WAIT
		};

	class StatusCheck {
    private:
       static int m_old_btn;
				static void mPlay(int motion_page,int mode = SOCCER,int wait = WAIT);
				static void resetLEDs(CM730 &cm730);
		public:
				static int m_cur_mode;
        static int m_is_started;
				static int m_current_walk_speed;
				static minIni* m_ini;
				static minIni* m_ini1;
				       
				static void Check(CM730 &cm730);

	};
}
#endif /* STATUSCHECK_H_ */
