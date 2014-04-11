#ifndef PLANNING_H
#define PLANNING_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

class planning
{
private:
	int write_command;

	int write_parameters;

public:
	int read_action;

	int read_velocity;

	int read_angle;

	void stop();

	void turn_left();

	void turn_right();

	void kick_left_strong();

	void kick_left_weak();

	void kick_right_strong();

	void kick_right_weak();

	void fall_right();

	void fall_left();

	void fall_forward();

	void fall_backward();

	void up_forward();

	void up_backward();

};
#endif
