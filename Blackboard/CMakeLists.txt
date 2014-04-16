#include_directories(${RobotFEI-HT_SOURCE_DIR}/Blackboard/include)
include_directories(include)
add_library(blackboard SHARED src/blackboard.cpp)
install(TARGETS blackboard DESTINATION ${RoboFEI-HT_BINARY_DIR}/lib)
#install(TARGETS blackboard DESTINATION /usr/local/lib)

