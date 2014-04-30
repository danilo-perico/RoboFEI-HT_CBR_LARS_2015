#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "blackboard.h"
#include "planning.h"


void planning::stop()
{
    write_command = 1;
    write_parameter_vel = 2;
    write_parameter_angle = 3;
    PLANNING_COMMAND = write_command;
    PLANNING_PARAMETER_VEL = write_parameter_vel;
    PLANNING_PARAMETER_ANGLE = write_parameter_angle;
}

void planning::turn_left()
{
    write_command = 4;
    write_parameter_vel = 5;
    write_parameter_angle = 6;
    PLANNING_COMMAND = write_command;
    PLANNING_PARAMETER_VEL = write_parameter_vel;
    PLANNING_PARAMETER_ANGLE = write_parameter_angle;
}

void planning::turn_right()
{
    write_command = 7;
    write_parameter_vel = 8;
    write_parameter_angle = 9;
    PLANNING_COMMAND = write_command;
    PLANNING_PARAMETER_VEL = write_parameter_vel;
    PLANNING_PARAMETER_ANGLE = write_parameter_angle;
}

void planning::kick_left_strong()
{
}

void planning::kick_left_weak()
{
}

void planning::kick_right_strong()
{
}

void planning::kick_right_weak()
{
}

void planning::fall_right()
{
}

void planning::fall_left()
{
}

void planning::fall_forward()
{
}

void planning::fall_backward()
{
}

void planning::up_forward()
{
}

void planning::up_backward()
{
}
