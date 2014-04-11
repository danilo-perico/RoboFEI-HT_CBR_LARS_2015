#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "../Include/blackboard.h"
#include "../Include/planning.h"


void planning::stop()
{
    write_command = 1;
    write_parameters = 2;
    DECISION_ACTION_A = write_command;
    DECISION_ACTION_B = write_parameters;
}

void planning::turn_left()
{
    write_command = 3;
    write_parameters = 4;
    DECISION_ACTION_A = write_command;
    DECISION_ACTION_B = write_parameters;
}

void planning::turn_right()
{
    write_command = 5;
    write_parameters = 6;
    DECISION_ACTION_A = write_command;
    DECISION_ACTION_B = write_parameters;
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
